/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniglesi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 21:46:34 by eniglesi          #+#    #+#             */
/*   Updated: 2022/02/16 21:46:36 by eniglesi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*a;
	int		n;

	n = nmemb * size;
	a = malloc(nmemb * size + 1);
	if (!a)
		return (NULL);
	while (n > 0)
	{
		n--;
		((char *)a)[n] = '\0';
	}
	return (a);
}

// Finds the index of the first newline character in a buffer.
// Returns the index or -1 if no newline is found.
static int	find_newline_index(char *buffer)
{
	int	i;

	i = 0;
	while (buffer[i] != '\n' && buffer[i])
		i++;
	if (buffer[i] == '\n')
		return (i);
	return (-1);
}

// Finalizes the line to be returned, updates the static buffer, and handles memory.
// current_line: The line content accumulated so far.
// static_buffer: The buffer for the current fd.
// line_len: The current length of content in current_line before adding a potential newline.
// free_line_flag: If 1, frees current_line and returns NULL (error/EOF case).
static char	*finalize_line_and_update_buffer(char *current_line, char *static_buffer, int line_len, int free_line_flag)
{
	int	i; // Index for newline in static_buffer
	int	j; // Length of remaining part in static_buffer

	i = 0;
	j = 0;
	if (free_line_flag == 1)
	{
		free(current_line);
		return (NULL);
	}
	if (static_buffer)
	{
		while (static_buffer[i] != '\n' && static_buffer[i])
			i++;
		while ((&static_buffer[i + 1])[j] != '\0') // Calculate length of content after newline
			j++;
		if (static_buffer[i] == '\n')
		{
			ft_strlcpy(static_buffer, &static_buffer[i + 1], j + 1); // Update static_buffer: shift content after newline to the beginning
			current_line[line_len] = '\n'; // Add newline to the current_line
			current_line[line_len + 1] = '\0'; // Explicitly null-terminate after adding newline
		}
		else if (!static_buffer[i]) // If no newline found in static_buffer (EOF reached in buffer)
			static_buffer[0] = '\0'; // Clear static_buffer
	}
	return (ft_realloc(current_line, 0)); // Resize current_line to its actual content length (+null terminator)
}

// Copies content from source_buffer to dest_string until a newline or end of source_buffer.
// Returns the number of characters copied (new length of dest_string part).
static int	copy_string_from_buffer(char *dest_string, char *source_buffer)
{
	int	aux; // Index and counter for characters copied

	aux = 0;
	while (source_buffer[aux] != '\n' && source_buffer[aux])
	{
		dest_string[aux] = source_buffer[aux];
		aux++;
	}
	return (aux);
}

char	*get_next_line(int fd)
{
	char			*string;
	// Array of static buffers, one for each possible file descriptor
	static char		extras[MAX_FD][BUFFER_SIZE + 1];
	int				baits; // Number of bytes read
	int				aux;   // Current length of the string being built

	// Check for invalid fd or BUFFER_SIZE
	if (fd < 0 || fd >= MAX_FD || BUFFER_SIZE <= 0)
		return (NULL);
	string = ft_calloc(sizeof(char), BUFFER_SIZE + 1);
	if (string == NULL)
		return (NULL);
	aux = copy_string_from_buffer(string, extras[fd]);
	if (extras[fd][aux]) // If a newline was found in extras[fd] by copy_string_from_buffer (indicated by non-null char after copied part)
		return (finalize_line_and_update_buffer(string, extras[fd], aux, 0));
	baits = 0; // Initialize baits
	while (find_newline_index(extras[fd]) == -1)
	{
		baits = leer(fd, extras[fd]);
		// Handle read errors or EOF
		if (baits < 0) { // read error
			free(string);
			extras[fd][0] = '\0'; // Clear the static buffer for this fd on error
			return (NULL);
		}
		if (baits == 0) { // EOF
			break;
		}
		string = ft_realloc(string, baits);
		if (string == NULL || extras[fd][0] == 0) // If realloc failed or buffer became empty unexpectedly
			return (finalize_line_and_update_buffer(string, extras[fd], aux, 1));
		aux = ft_copynl(extras[fd], baits, string, aux); // ft_copynl is from utils, appends from extras[fd] to string
	}
	// If EOF was hit AND string is empty AND no newline in buffer (extras[fd] is also empty or no newline)
	if (baits == 0 && !string[0] && find_newline_index(extras[fd]) == -1)
		return (finalize_line_and_update_buffer(string, extras[fd], aux, 1));
	return (finalize_line_and_update_buffer(string, extras[fd], aux, 0));
}
