/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manag.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkanaev <alkanaev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 14:53:03 by alkanaev          #+#    #+#             */
/*   Updated: 2020/12/19 16:31:12 by alkanaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

void	arr_cleaner(char **str)
{
	int		i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
	str = NULL;
}

//search for
int			ind_of_envvar(t_command *mimi, char *var)
{
	int		i;
	char	*varenv;
	char	**sep;
    int		len;
	
    sep = split_mod(var, "=");
	varenv = ft_strdup(sep[0]);
	arr_cleaner(sep);
	len = ft_strlen(varenv);
	i = 0;
	while (mimi->env_arr[i])
	{
		if (!strncmp(mimi->env_arr[i], varenv, len))
		{
			free(varenv);
			return (i);
		}
		i++;
	}
	free(varenv);
	return (-1);
}

void		del_envvar(t_command *mimi, char *var)
{
	int		i;
	int		j;
	int		ind;
	char	**tmp; //for env
	int		len;

	i = 0;
	j = 0;
    ind = ind_of_envvar(mimi, var);
	if (ind == -1)
		return ;
	len = cnt_com_parts(mimi->env_arr);
	if (!(tmp = (char **)ft_calloc(sizeof(char *), len + 1)))
		return ;
	while (mimi->env_arr[i])
	{
		if (i != ind)
		{
			tmp[j] = ft_strdup(mimi->env_arr[i]);
			j++;
		}
		i++;
	}
	arr_cleaner(mimi->env_arr);
	mimi->env_arr = tmp;
}

//update variables from env_arr // if var already exist - we remove it first
void		envvar_update(t_command *mimi, char *var)
{
	char	**tmp;
	int		i;
	int		j;
	int		len;

    i = 0;
	j = 0;
	del_envvar(mimi, var);
	len = cnt_com_parts(mimi->env_arr);
	tmp = (char **)ft_calloc(sizeof(char *), len + 2);
	if (tmp == NULL)
		return ;
	while (mimi->env_arr[i])
	{
		tmp[j] = ft_strdup(mimi->env_arr[i]);
		i++;
		j++;
	}
	tmp[j] = ft_strdup(var);
	arr_cleaner(mimi->env_arr);
	mimi->env_arr = tmp;
	strdel(&var);
}

//to manage envvar_pr_sort (part of sorting)
void		envvar_sort(char **env_arr, int len)
{
	int		k;
	char	*tmp;
    int		sort;

	sort = 0;
	while (env_arr && sort == 0)
	{
		sort = 1;
		k = 0;
		while (k < len - 1)
		{
			if (ft_strcmp(env_arr[k], env_arr[k + 1]) > 0)
			{
				tmp = env_arr[k];
				env_arr[k] = env_arr[k + 1];
				env_arr[k + 1] = tmp;
				sort = 0;
			}
			k++;
		}
		len--;
	}
}

char		*ft_strndup(char *str, long len)
{
	char *tmp;
	long i;

	i = 0;
	tmp = NULL;
	tmp = (char *)malloc(sizeof(char) * len + 1);
	if (!tmp)
		return (NULL);
	tmp[len] = '\0';
	while (i < len)
	{
		tmp[i] = str[i];
		i++;
	}
	return (tmp);
}

//to manage envvar_pr_sort (part of printing)
void		envvar_print(char *env_arr)
{
	char	*var;
	char	*val;
	int		i;

	i = 0;
    val = NULL;
	while (env_arr[i] && env_arr[i] != '=')
		i++;
	var = ft_strndup(env_arr, i);
	if (env_arr[i] == '=')
		val = ft_strdup(&env_arr[++i]);
	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	ft_putstr_fd(var, STDOUT_FILENO);
	if (val)
	{
		ft_putstr_fd("=\"", STDOUT_FILENO);
		ft_putstr_fd(val, STDOUT_FILENO);
		ft_putstr_fd("\"", STDOUT_FILENO);
	}
	ft_putstr_fd("\n", STDOUT_FILENO);
	strdel(&val);
	strdel(&var);
}

//Finally print sorted env XD
void		envvar_pr_sort(t_command *mimi)
{
	int		argc;
	char	**tmp;
    int		i;
	int		j;

	i = 0;
	j = 0;
    argc = cnt_com_parts(mimi->env_arr);
	tmp = (char **)ft_calloc(sizeof(char *), argc + 1);
	while (i < argc)
	{
		tmp[j] = ft_strdup(mimi->env_arr[i]);
		i++;
		j++;
	}
	envvar_sort(tmp, cnt_com_parts(tmp));
	j = 0;
	while (j < argc)
	{
		envvar_print(tmp[j]);
		j++;
	}
	arr_cleaner(tmp);
	mimi->ret = 0;
}

void	strdel(char **str)
{
	if (str)
	{
		free(*str);
		*str = NULL;
	}
}