/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniglesi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 12:47:28 by eniglesi          #+#    #+#             */
/*   Updated: 2022/01/13 12:47:31 by eniglesi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	a;
	size_t	i;

	i = 0;
	while (src[i] != '\0')
		i++;
	if (!dst || !src)
		return (0);
	if (size == 0)
		return (i);
	a = 0;
	while (a < (size - 1) && src[a] != '\0')
	{
		dst[a] = src[a];
		a++;
	}
	dst[a] = '\0';
	return (i);
}

char	*ft_realloc(char *s1, int j)
{
	char	*a;
	int		new_total_len;
	int		old_len;
	int		k;

	old_len = 0;
	while (s1[old_len] != '\0') // Calculate old length of s1
		old_len++;

	new_total_len = old_len + j; // Calculate new desired total length

	if (new_total_len <= 0) // If new length is zero or negative
	{
		free(s1); // Free the original buffer
		return (NULL); // Return NULL
	}

	a = malloc(sizeof(char) * new_total_len + 1); // Allocate memory for new_total_len + null terminator
	if (!a)
	{
		// Malloc failed. Do NOT free s1. Return NULL.
		return (NULL);
	}

	// Zero out the newly allocated memory block 'a'
	k = 0;
	while (k < new_total_len + 1)
	{
		a[k] = '\0';
		k++;
	}

	ft_strlcpy(a, s1, old_len + 1); // Copy content from s1 to a (up to old_len characters)
	free(s1); // Free the old buffer s1
	return (a); // Return the new buffer
}

// Copies from 'extra' (source) to 'string' (destination) starting at string[aux].
// Copies up to 'baits' characters or until a newline is found.
// If a newline is found, it IS copied to 'string'.
// Returns the new total length of 'string' (updated 'aux').
int	ft_copynl(char *extra, int baits, char *string, int aux)
{
	int	j; // Index for 'extra'

	j = 0;
	while (j < baits) // Iterate up to 'baits' characters from 'extra'
	{
		string[aux] = extra[j];
		if (extra[j] == '\n')
		{
			aux++; // Include newline in string and its length count
			// j++; // Increment j to mark newline as processed by this iteration, though loop breaks
			break; // Stop after copying newline
		}
		aux++;
		j++;
	}
	// Note: This function does not null-terminate 'string'.
	// The overall buffer 'string' is managed and eventually null-terminated
	// by the caller or subsequent functions (e.g., finalize_line_and_update_buffer).
	return (aux);
}

int	leer(int fd, char *extra_buffer) // Renamed 'extra' to 'extra_buffer' for clarity
{
	int bytes_read;

	bytes_read = read(fd, extra_buffer, BUFFER_SIZE);
	if (bytes_read > 0)
	{
		extra_buffer[bytes_read] = '\0';
	}
	// If bytes_read is 0 (EOF), the buffer is not explicitly changed here.
	// The caller (get_next_line) handles the EOF condition.
	// If bytes_read is -1 (error), the content of extra_buffer is indeterminate
	// and should not be written to. The error will be handled by the caller.
	return bytes_read;
}
