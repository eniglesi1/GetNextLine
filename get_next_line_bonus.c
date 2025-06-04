/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniglesi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 21:46:34 by eniglesi          #+#    #+#             */
/*   Updated: 2023/11/15 XXXXXX by a-swe-agent      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

#define MAX_FD 1024

// ft_calloc from get_next_line.c (corrected version)
static void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*a;
	size_t	n;

	n = nmemb * size;
	a = malloc(n);
	if (!a)
		return (NULL);

	unsigned char *p = (unsigned char *)a;
	size_t i = 0;
	while (i < n) {
		p[i] = '\0';
		i++;
	}
	return (a);
}

// find_newline_in_buffer from get_next_line.c
static int	find_newline_in_buffer(char *buffer)
{
	int	i;

	i = 0;
	while (buffer[i] != '\0')
	{
		if (buffer[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

// copy_from_buffer_to_string from get_next_line.c
static int	copy_from_buffer_to_string(char *current_line_buffer, char *static_buffer_for_fd)
{
	int	i;

	i = 0;
	while (static_buffer_for_fd[i] != '\0' && i < BUFFER_SIZE)
	{
		current_line_buffer[i] = static_buffer_for_fd[i];
		if (static_buffer_for_fd[i] == '\n')
		{
			i++;
			break;
		}
		i++;
	}
	current_line_buffer[i] = '\0';
	return (i);
}

// finalize_line_and_update_buffer from get_next_line.c
static char	*finalize_line_and_update_buffer(char *current_line_buffer, char *static_buffer_for_fd, int current_line_len, int error_or_eof_flag)
{
	char	*returned_line;
	int		nl_idx_in_static;

	if (error_or_eof_flag == 1)
	{
		free(current_line_buffer);
		return (NULL);
	}

	if (current_line_len == 0) {
		free(current_line_buffer);
		return (NULL);
	}

	if (current_line_len > 0 && current_line_buffer[current_line_len - 1] == '\n')
	{
		nl_idx_in_static = find_newline_in_buffer(static_buffer_for_fd);
		if (nl_idx_in_static != -1)
		{
			ft_strlcpy(static_buffer_for_fd, &static_buffer_for_fd[nl_idx_in_static + 1], BUFFER_SIZE + 1);
		} else {
			static_buffer_for_fd[0] = '\0';
		}
	}
	else
	{
		static_buffer_for_fd[0] = '\0';
	}

	returned_line = (char *)malloc(sizeof(char) * (current_line_len + 1));
	if (!returned_line)
	{
		free(current_line_buffer);
		static_buffer_for_fd[0] = '\0';
		return (NULL);
	}
	ft_strlcpy(returned_line, current_line_buffer, current_line_len + 1);

	free(current_line_buffer);
	return (returned_line);
}

// get_next_line function adapted for bonus
char	*get_next_line(int fd)
{
	char			*current_line_buffer;
	static char		static_read_buffer[MAX_FD][BUFFER_SIZE + 1];
	int				bytes_read;
	int				current_line_len;
	char			*temp_ptr;

	if (fd < 0 || fd >= MAX_FD || BUFFER_SIZE <= 0) // Added MAX_FD check
		return (NULL);

	current_line_buffer = (char *)ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!current_line_buffer)
		return (NULL);

	current_line_len = copy_from_buffer_to_string(current_line_buffer, static_read_buffer[fd]);

	if (current_line_len > 0 && current_line_buffer[current_line_len - 1] == '\n')
	{
		return (finalize_line_and_update_buffer(current_line_buffer, static_read_buffer[fd], current_line_len, 0));
	}

	while (!(current_line_len > 0 && current_line_buffer[current_line_len - 1] == '\n'))
	{
		bytes_read = leer(fd, static_read_buffer[fd]);

		if (bytes_read == -1)
		{
			free(current_line_buffer);
			static_read_buffer[fd][0] = '\0'; // Clear buffer for specific fd
			return (NULL);
		}

		if (bytes_read == 0)
		{
			return (finalize_line_and_update_buffer(current_line_buffer, static_read_buffer[fd], current_line_len, 0));
		}

		temp_ptr = ft_realloc(current_line_buffer, bytes_read);
		if (!temp_ptr) {
			free(current_line_buffer);
			static_read_buffer[fd][0] = '\0'; // Clear buffer for specific fd
			return (NULL);
		}
		current_line_buffer = temp_ptr;

		current_line_len = ft_copynl(static_read_buffer[fd], bytes_read, current_line_buffer, current_line_len);
	}

	return (finalize_line_and_update_buffer(current_line_buffer, static_read_buffer[fd], current_line_len, 0));
}
