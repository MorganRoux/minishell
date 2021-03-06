/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkanaev <alkanaev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 09:58:09 by alkanaev          #+#    #+#             */
/*   Updated: 2021/02/09 10:20:33 by alkanaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			is_token_error(t_list_str *tokens)
{
	t_list_str	*prev_token;

	prev_token = tokens;
	tokens = tokens->next;
	while (tokens != NULL && is_blank(tokens->content))
		tokens = tokens->next;
	while (tokens != NULL)
	{
		if (is_meta_str(prev_token->content) && is_meta_str(tokens->content))
			return (display_error((char *)tokens->content));
		prev_token = tokens;
		tokens = tokens->next;
		while (tokens != NULL && is_blank(tokens->content))
			tokens = tokens->next;
	}
	if (is_meta_str(prev_token->content)
		&& ft_strcmp(prev_token->content, ";") != 0)
		return (display_error((char *)prev_token->content));
	return (0);
}

int			is_first_error(t_list_str *tokens)
{
	if (tokens == NULL)
		return (0);
	if (((char *)tokens->content)[0] == '|')
	{
		display_error((char *)tokens->content);
		return (1);
	}
	return (0);
}

int			is_syntax_error(t_list_str *tokens)
{
	if (is_first_error(tokens) ||
		is_token_error(tokens))
		return (1);
	return (0);
}

t_list_cmd	*parse(char *line, t_command *global_command)
{
	t_list_cmd	*cmds;
	t_list_str	*tokens;

	cmds = NULL;
	if ((tokens = split_tokens(line)))
		cmds = parse_tokens(tokens, global_command);
	ft_lstclear(&tokens, free);
	return (cmds);
}

int			check_errors(char *line, t_command *global_command)
{
	t_list_str	*tokens;

	if ((tokens = split_tokens(line)))
	{
		if (!is_syntax_error(tokens))
		{
			ft_lstclear(&tokens, free);
			return (0);
		}
	}
	ft_lstclear(&tokens, free);
	global_command->ret = 2;
	return (1);
}
