#include "minishell.h"

t_command	g_globstruct;

int     is_meta_char(char c)
{
    char    METACHARACTER[] = " \t\n|&;()<>$";
    int     i;

    i = 0;
    while (METACHARACTER[i] != 0)
    {
        if (METACHARACTER[i] == c)
            return(1);
        i++;
    }
    return (0);
}

int     is_meta_str(char *c)
{
    char    METACHARACTER[] = " \t\n|&;()<>$";
    int     i;

    i = 0;
    //printf("\nc - [[%s]]\n", c);
    while (METACHARACTER[i] != 0)
    {
        if (METACHARACTER[i] == c[0])
            return (1);
        i++;
    }
    return (0);
}

int     is_space_str(char *c)
{
    char    METACHARACTER[] = " \t";
    int     i;

    i = 0;
    while (METACHARACTER[i] != 0)
    {
        if (METACHARACTER[i] == c[0])
            return (1);
        i++;
    }
    return (0);
}