/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkanaev <alkanaev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 09:50:50 by alkanaev          #+#    #+#             */
/*   Updated: 2021/01/30 09:50:51 by alkanaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_paths(char *envp[])
{
	char	*var;
	char	**paths;
	char	**ret;
	int		len;

	len = 0;
	var = get_var(envp, "PATH");
	if (*var == 0)
	{
		free(var);
		return (NULL);
	}
	paths = ft_split(var, ':');
	while (paths[len] != 0)
		len++;
	ret = malloc((len + 1) * sizeof(char*));
	len = -1;
	while (paths[++len] != 0)
		ret[len] = ft_strjoin(paths[len], "/");
	ret[len] = 0;
	free(var);
	free_strs(paths);
	return (ret);
}
