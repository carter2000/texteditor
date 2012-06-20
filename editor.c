#include "editor.h"

#include <assert.h>
#include <malloc.h>
#include <stdio.h>

#include "hstring.h"

#define MAX_LEN     25  // �ļ�������� 
#define LINE_LEN    75  // ÿ���ַ������ֵ+1
#define NAME_LEN    20  // �ļ�����󳤶�(�����̷���·��)+1
#define MENU_COUNT  10  // �˵�����

static int g_line_count = 0;
static HString g_lines[MAX_LEN];
static char g_filename[NAME_LEN];
static int g_isopen = 0;

static void _editor_init()
{
    int index = 0;
    for (index = 0; index < MAX_LEN; ++index) {
        hstr_init(&g_lines[index]);
    }
    g_line_count = 0;
    g_filename[0] = '\0';
    g_isopen = 1;
}

static void _editor_clear()
{
    int index = 0;
    for (index = 0; index < MAX_LEN; ++index) {
        hstr_clear(&g_lines[index]);
    }
    g_line_count = 0;
    g_filename[0] = '\0';
    g_isopen = 0;
}

static void _editor_hint()
{
    printf("��ѡ��1.���ļ�(�»��)  2.��ʾ�ļ�����\n");
    printf("       3.������  4.ɾ����  5.������  6.�޸���\n");
    printf("       7.�����ַ���        8.�滻�ַ���\n");
    printf("       9.�����˳�          0.�����༭\n");
}

static int _editor_action()
{
    int action = -1;
    while (action < 0 || action >= MENU_COUNT) {
        scanf("%d", &action);
    }
    return action;
}

static int _editor_discard()
{
    _editor_clear();
    return -1;
}

static int _editor_open()
{
    FILE* file = NULL;
    char content[LINE_LEN] = {0};

    if (g_isopen) {
        printf("�Ѿ��д򿪵��ļ�������ִ��'9.�����˳� 0.�����༭'\n");
    }
    else {
        _editor_init();

        printf("�������ļ���(�ɰ����̷���·����������19���ַ�): ");
        scanf("%s", g_filename);

        file = fopen(g_filename, "r");
        if (file != NULL) {
            while (fgets(content, LINE_LEN, file) != NULL) {
                hstr_assign(&g_lines[g_line_count], content);
                if (++g_line_count > MAX_LEN) {
                    break;
                }
            }
            fclose(file);
        }
    }

    return 0;
}

static int _editor_show()
{
    int index = 0;

    if (!g_isopen) {
        printf("����ִ��'1.���ļ�(�»��)'\n");
        return 0;
    }

    for (index = 0; index < g_line_count; ++index) {
        printf("%d: ", index + 1);
        hstr_print(&g_lines[index]);
    }

    return 0;
}

static int _editor_insert()
{
    int pos = -1;
    int count = -1;
    int index = 0;
    char line[LINE_LEN] = {0};

    if (!g_isopen) {
        printf("����ִ��'1.���ļ�(�»��)'\n");
        return 0;
    }

    while (pos < 0 || count < 0) {
        printf("�ڵ�l��ǰ����m�У������� l m: ");
        scanf("%d%d", &pos, &count);
        if ((pos <= 0 && g_line_count > 0) || pos > g_line_count) {
            pos = -1;
            printf("�Ƿ��� l\n");
        }
        else if (count <= 0 || count + g_line_count > MAX_LEN) {
            count = -1;
            printf("�Ƿ��� m\n");
        }
    }
    for (index = g_line_count - 1; index >= pos - 1 && index >= 0; --index) {
        hstr_copy(&g_lines[index], &g_lines[index + count]);
    }
    g_line_count += count;

    printf("��˳���������������:\n");
    index = pos;
    while (count-- > 0) {
        fflush(stdin);
        fgets(line, LINE_LEN, stdin);
        hstr_assign(&g_lines[index++], line);
    }

    return 0;
}

static int _editor_delete()
{
    int index = 0;
    int pos = -1;
    int count = -1;

    if (!g_isopen) {
        printf("����ִ��'1.���ļ�(�»��)'\n");
        return 0;
    }

    while (pos < 0 || count < 0) {
        printf("�ӵ�l����ɾ��m�У������� l m: ");
        scanf("%d%d", &pos, &count);
        if (pos <= 0 || pos > g_line_count) {
            pos = -1;
            printf("�Ƿ��� l\n");
        }
        else if (count <= 0 || count + pos - 1 > g_line_count) {
            count = -1;
            printf("�Ƿ��� m\n");
        }
    }

    for (index = pos - 1; index < g_line_count - 1; ++index) {
        hstr_copy(&g_lines[index + count], &g_lines[index]);
    }

    for (index = g_line_count - count; index < g_line_count; ++index) {

        hstr_clear(&g_lines[index]);
    }
    g_line_count -= count;

    return 0;
}

static int _editor_copy()
{
    int l = -1;
    int m = -1;
    int k = -1;
    int index = 0;
    HString* mlines = NULL;

    if (!g_isopen) {
        printf("����ִ��'1.���ļ�(�»��)'\n");
        return 0;
    }

    while (l < 0 || m < 0 || k < 0) {
        printf("�ѵ�l�п�ʼ��m�в���ԭk��֮ǰ�������� l m k: ");
        scanf("%d%d%d", &l, &m, &k);
        if (l <= 0 || l >= g_line_count) {
            l = -1;
            printf("�Ƿ��� l\n");
        }
        else if (m <= 0 || m + l - 1 > g_line_count 
            || m + g_line_count > MAX_LEN) {
            m = -1;
            printf("�Ƿ��� m\n");
        }
        else if (k <= 0 || k > g_line_count) {
            k = -1;
            printf("�Ƿ��� k\n");
        }
    }

    mlines = (HString*)malloc(m * sizeof(HString));
    for (index = 0; index < m; ++index) {
        hstr_init(&mlines[index]);
        hstr_copy(&g_lines[index + l - 1], &mlines[index]);
    }

    for (index = g_line_count - 1; index >= k - 1; --index) {
        hstr_copy(&g_lines[index], &g_lines[index + m]);
    }
    g_line_count += m;

    for (index = 0; index < m; ++index) {
        hstr_copy(&mlines[index], &g_lines[index + k - 1]);
        hstr_clear(&mlines[index]);
    }
    free(mlines);

    return 0;
}

static int _editor_modify()
{
    int line = -1;
    char content[LINE_LEN] = {0};

    if (!g_isopen) {
        printf("����ִ��'1.���ļ�(�»��)'\n");
        return 0;
    }

    while (line < 0) {
        printf("��������޸ĵ��к�: ");
        scanf("%d", &line);
        if (line <= 0 || line > g_line_count) {
            line = -1;
            printf("�Ƿ����к�\n");
        }
    }

    printf("%d: ", line);
    hstr_print(&g_lines[line - 1]);

    printf("������������: ");
    fflush(stdin);
    fgets(content, LINE_LEN, stdin);
    hstr_assign(&g_lines[line - 1], content);

    return 0;
}

static int _editor_find()
{
    int pos = 0;
    int index = 0;
    char option = 0;
    HString tmp;
    char content[LINE_LEN] = {0};

    if (!g_isopen) {
        printf("����ִ��'1.���ļ�(�»��)'\n");
        return 0;
    }

    hstr_init(&tmp);

    printf("����������ҵ��ַ���: ");
    scanf("%s", content);
    hstr_assign(&tmp, content);

    for (index = 0; index < g_line_count; ++index) {
        pos = hstr_find(&g_lines[index], &tmp);
        if (pos != 0) {
            printf("��%d��: ", index + 1);
            hstr_print(&g_lines[index]);
            printf("��%d���ַ����ҵ�������������(Y/N)? ", pos);
            fflush(stdin);
            scanf("%c", &option);
            if (option != 'Y' && option != 'y') {
                break;
            }
        }
    }

    if (index == g_line_count) {
        printf("û�ҵ�\n");
    }

    hstr_clear(&tmp);

    return 0;
}

static int _editor_replace()
{
    int pos = 0;
    int index = 0;
    char option = 0;
    HString strsub;
    HString strrep;
    char sub[LINE_LEN] = {0};
    char rep[LINE_LEN] = {0};

    if (!g_isopen) {
        printf("����ִ��'1.���ļ�(�»��)'\n");
        return 0;
    }

    hstr_init(&strsub);
    hstr_init(&strrep);

    printf("����������ҵ��ַ���: ");
    scanf("%s", sub);
    hstr_assign(&strsub, sub);

    printf("�滻Ϊ: ");
    scanf("%s", rep);
    hstr_assign(&strrep, rep);

    for (index = 0; index < g_line_count; ++index) {
        pos = hstr_find(&g_lines[index], &strsub);
        if (pos != 0) {
            printf("��%d��: ", index + 1);
            hstr_print(&g_lines[index]);
            printf("��%d���ַ����ҵ����Ƿ��滻(Y/N)? ", pos);
            fflush(stdin);
            scanf("%c", &option);
            if (option == 'Y' || option == 'y') {
                hstr_replace(&g_lines[index], &strsub, &strrep);
                printf("�����滻��(Y/N)? ");
                fflush(stdin);
                scanf("%c", &option);
                if (option != 'Y' && option != 'y') {
                    break;
                }
            }
        }
    }

    if (index == g_line_count) {
        printf("û�ҵ�\n");
    }

    hstr_clear(&strsub);
    hstr_clear(&strrep);

    return 0;
}

static int _editor_save()
{
    int index = 0;
    FILE* file = NULL;

    if (!g_isopen) {
        printf("����ִ��'1.���ļ�(�»��)'\n");
        return 0;
    }

    file = fopen(g_filename, "w");
    if (file == NULL) {
        printf("�޷����ļ�\n");
        return 0;
    }

    for (index = 0; index < g_line_count; ++index) {
        if (hstr_empty(&g_lines[index])) {
            fwrite("\n", sizeof(char), 1, file);
        }
        else {
            fwrite(g_lines[index].ch, sizeof(char), 
                   g_lines[index].length, file);
        }
    }

    fclose(file);

    return -1;
}

typedef int (*EditorAction)();
static EditorAction g_actions[MENU_COUNT] = {
    &_editor_discard,
    &_editor_open,
    &_editor_show,
    &_editor_insert,
    &_editor_delete,
    &_editor_copy,
    &_editor_modify,
    &_editor_find,
    &_editor_replace,
    &_editor_save,
};
void editor_loop()
{
    int action = 0;
    do 
    {
        _editor_hint();
        action = _editor_action();
        assert(action >= 0 && action < MENU_COUNT);
    } while ((*g_actions[action])() != -1);

    system("pause");
}