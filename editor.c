#include "editor.h"

#include <assert.h>
#include <malloc.h>
#include <stdio.h>

#include "hstring.h"

#define MAX_LEN     25  // 文件最大行数 
#define LINE_LEN    75  // 每行字符数最大值+1
#define NAME_LEN    20  // 文件名最大长度(包括盘符、路径)+1
#define MENU_COUNT  10  // 菜单个数

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
    printf("请选择：1.打开文件(新或旧)  2.显示文件内容\n");
    printf("       3.插入行  4.删除行  5.拷贝行  6.修改行\n");
    printf("       7.查找字符串        8.替换字符串\n");
    printf("       9.存盘退出          0.放弃编辑\n");
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
        printf("已经有打开的文件，请先执行'9.存盘退出 0.放弃编辑'\n");
    }
    else {
        _editor_init();

        printf("请输入文件名(可包括盘符、路径，不超过19个字符): ");
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
        printf("请先执行'1.打开文件(新或旧)'\n");
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
        printf("请先执行'1.打开文件(新或旧)'\n");
        return 0;
    }

    while (pos < 0 || count < 0) {
        printf("在第l行前插入m行，请输入 l m: ");
        scanf("%d%d", &pos, &count);
        if ((pos <= 0 && g_line_count > 0) || pos > g_line_count) {
            pos = -1;
            printf("非法的 l\n");
        }
        else if (count <= 0 || count + g_line_count > MAX_LEN) {
            count = -1;
            printf("非法的 m\n");
        }
    }
    for (index = g_line_count - 1; index >= pos - 1 && index >= 0; --index) {
        hstr_copy(&g_lines[index], &g_lines[index + count]);
    }
    g_line_count += count;

    printf("请顺序输入待插入内容:\n");
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
        printf("请先执行'1.打开文件(新或旧)'\n");
        return 0;
    }

    while (pos < 0 || count < 0) {
        printf("从第l行起删除m行，请输入 l m: ");
        scanf("%d%d", &pos, &count);
        if (pos <= 0 || pos > g_line_count) {
            pos = -1;
            printf("非法的 l\n");
        }
        else if (count <= 0 || count + pos - 1 > g_line_count) {
            count = -1;
            printf("非法的 m\n");
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
        printf("请先执行'1.打开文件(新或旧)'\n");
        return 0;
    }

    while (l < 0 || m < 0 || k < 0) {
        printf("把第l行开始的m行插在原k行之前，请输入 l m k: ");
        scanf("%d%d%d", &l, &m, &k);
        if (l <= 0 || l >= g_line_count) {
            l = -1;
            printf("非法的 l\n");
        }
        else if (m <= 0 || m + l - 1 > g_line_count 
            || m + g_line_count > MAX_LEN) {
            m = -1;
            printf("非法的 m\n");
        }
        else if (k <= 0 || k > g_line_count) {
            k = -1;
            printf("非法的 k\n");
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
        printf("请先执行'1.打开文件(新或旧)'\n");
        return 0;
    }

    while (line < 0) {
        printf("请输入待修改的行号: ");
        scanf("%d", &line);
        if (line <= 0 || line > g_line_count) {
            line = -1;
            printf("非法的行号\n");
        }
    }

    printf("%d: ", line);
    hstr_print(&g_lines[line - 1]);

    printf("请输入新内容: ");
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
        printf("请先执行'1.打开文件(新或旧)'\n");
        return 0;
    }

    hstr_init(&tmp);

    printf("请输入待查找的字符串: ");
    scanf("%s", content);
    hstr_assign(&tmp, content);

    for (index = 0; index < g_line_count; ++index) {
        pos = hstr_find(&g_lines[index], &tmp);
        if (pos != 0) {
            printf("第%d行: ", index + 1);
            hstr_print(&g_lines[index]);
            printf("第%d个字符处找到。继续查找吗(Y/N)? ", pos);
            fflush(stdin);
            scanf("%c", &option);
            if (option != 'Y' && option != 'y') {
                break;
            }
        }
    }

    if (index == g_line_count) {
        printf("没找到\n");
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
        printf("请先执行'1.打开文件(新或旧)'\n");
        return 0;
    }

    hstr_init(&strsub);
    hstr_init(&strrep);

    printf("请输入待查找的字符串: ");
    scanf("%s", sub);
    hstr_assign(&strsub, sub);

    printf("替换为: ");
    scanf("%s", rep);
    hstr_assign(&strrep, rep);

    for (index = 0; index < g_line_count; ++index) {
        pos = hstr_find(&g_lines[index], &strsub);
        if (pos != 0) {
            printf("第%d行: ", index + 1);
            hstr_print(&g_lines[index]);
            printf("第%d个字符处找到。是否替换(Y/N)? ", pos);
            fflush(stdin);
            scanf("%c", &option);
            if (option == 'Y' || option == 'y') {
                hstr_replace(&g_lines[index], &strsub, &strrep);
                printf("继续替换吗(Y/N)? ");
                fflush(stdin);
                scanf("%c", &option);
                if (option != 'Y' && option != 'y') {
                    break;
                }
            }
        }
    }

    if (index == g_line_count) {
        printf("没找到\n");
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
        printf("请先执行'1.打开文件(新或旧)'\n");
        return 0;
    }

    file = fopen(g_filename, "w");
    if (file == NULL) {
        printf("无法打开文件\n");
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