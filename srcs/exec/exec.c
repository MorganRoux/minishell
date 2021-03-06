/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkanaev <alkanaev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 14:23:44 by alkanaev          #+#    #+#             */
/*   Updated: 2021/02/09 09:56:24 by alkanaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		do_built_ins(t_command *global_cmd, char **cmd)
{
	if (!ft_strcmp(cmd[0], "echo"))
		com_echo(global_cmd, cmd);
	else if (!ft_strcmp(cmd[0], "cd"))
		com_cd(global_cmd, cmd);
	else if (!ft_strcmp(cmd[0], "env"))
		com_env(global_cmd);
	else if (!ft_strcmp(cmd[0], "pwd"))
		com_pwd(global_cmd);
	else if (!ft_strcmp(cmd[0], "exit"))
		com_exit(global_cmd, cmd);
	else if (!ft_strcmp(cmd[0], "unset"))
		com_unset(global_cmd, cmd);
	else if (!ft_strcmp(cmd[0], "export"))
		com_export(global_cmd, cmd);
	else
		return (1);
	return (global_cmd->ret);
}

int		exec_built_ins(t_command *global_cmd, char **cmd, t_command *cur_cmd)
{
	int		saved_stdin;
	int		saved_stdout;

	(void)cmd;
	apply_redirections_in(cur_cmd);
	apply_pipe_in(cur_cmd);
	close(cur_cmd->flux_in[1]);
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (is_redirection_in(cur_cmd) || is_pipe_in(cur_cmd))
		dup2(cur_cmd->flux_in[0], STDIN_FILENO);
	if (is_redirection_out(cur_cmd) || is_pipe_out(cur_cmd))
		dup2(cur_cmd->flux_out[1], STDOUT_FILENO);
	global_cmd->ret = do_built_ins(global_cmd, cmd);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(cur_cmd->flux_out[1]);
	close(cur_cmd->flux_in[0]);
	apply_redirections_out(cur_cmd);
	return (1);
}

int		is_built_in(char **cmd)
{
	if (!ft_strcmp(cmd[0], "echo") ||
		!ft_strcmp(cmd[0], "cd") ||
		!ft_strcmp(cmd[0], "env") ||
		!ft_strcmp(cmd[0], "pwd") ||
		!ft_strcmp(cmd[0], "exit") ||
		!ft_strcmp(cmd[0], "unset") ||
		!ft_strcmp(cmd[0], "export"))
		return (1);
	return (0);
}

int		exec(t_command *global_cmd, t_list_cmd *cmds)
{
	char	**cmd;

	while (cmds != NULL)
	{
		cmd = extract_command_and_args(cmds->content);
		if (cmd[0] == 0)
			global_cmd->ret = 127;
		if (open_redirections(cmds) == -1)
		{
			global_cmd->ret = 1;
			free_strs(cmd);
			return (-1);
		}
		if (is_built_in(cmd))
			exec_built_ins(global_cmd, cmd, cmds->content);
		else
			exec_command(cmds->content, global_cmd);
		cmds = cmds->next;
		free_strs(cmd);
	}
	return (0);
}

void	exec_loop(char *line, t_command *global_command)
{
	char		**command_list;
	int			i;
	t_list_cmd	*cmds;
	t_list_cmd	*new_cmd;

	i = 0;
	cmds = NULL;
	if (check_errors(line, global_command))
		return ;
	command_list = split_commands(line);
	while (command_list[i] != NULL)
	{
		command_list[i] = solve_dollards(command_list[i], global_command);
		new_cmd = parse(command_list[i], global_command);
		if (cmds != NULL)
			link_commands(ft_lstlast(cmds), new_cmd);
		ft_lstadd_back(&cmds, new_cmd);
		if (exec(global_command, ft_lstlast(cmds)) == -1)
			break ;
		i++;
	}
	free_strs(command_list);
	free_cmds(cmds);
}
