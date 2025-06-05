/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniglesi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 21:49:20 by eniglesi          #+#    #+#             */
/*   Updated: 2023/XX/XX XX:XX:XX by Your Name         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <stdlib.h> // For malloc, free
#include <stddef.h> // For size_t (already used by ft_strlcpy)

// local_calloc: A custom calloc implementation for ft_realloc.
// Handles (0,0) allocation like malloc(0) by ensuring at least 1 byte for a null terminator.
// Performs overflow check for nmemb * size.
static void	*local_calloc(size_t nmemb, size_t size)
{
    void	*a;
    size_t	total_size;
    size_t  i;

    // Handle calloc(0,0) or other zero-size cases like malloc(0) -> min alloc
    // Ensures space for at least a null terminator.
    if (nmemb == 0 || size == 0) {
         nmemb = 1;
         size = 1;
    }

    total_size = nmemb * size;
    if (size != 0 && total_size / size != nmemb) // Overflow check
        return (NULL);

    a = malloc(total_size);
    if (!a)
        return (NULL);
    i = 0;
    // Zero out the allocated memory
    while (i < total_size)
    {
        ((char *)a)[i] = '\0'; // Corrected null terminator
        i++;
    }
    return (a);
}

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

// ft_realloc: Reallocates memory for s1 to be old_len + size_to_add.
// If s1 is NULL, behaves like malloc.
// If allocation fails, returns NULL and s1 is NOT freed (caller responsibility).
// On success, frees s1 and returns the new pointer.
char	*ft_realloc(char *s1, int size_to_add)
{
    char	*new_ptr;
    int		old_len;
    int		new_total_len;

    if (!s1)
    {
        // If s1 is NULL, behave like malloc for (size_to_add + 1) bytes.
        // Ensure size_to_add is non-negative.
        if (size_to_add < 0) size_to_add = 0;
        return (local_calloc(size_to_add + 1, sizeof(char)));
    }

    old_len = 0;
    while (s1[old_len])
        old_len++;

    new_total_len = old_len + size_to_add;

    // If new_total_len would be negative, calculate as 0.
    // local_calloc will handle 0-size allocation by allocating 1 byte.
    if (new_total_len < 0) {
        new_total_len = 0;
    }

    // Allocate new memory. local_calloc ensures new_total_len + 1 for null terminator.
    new_ptr = local_calloc(new_total_len + 1, sizeof(char));

    if (!new_ptr) // If local_calloc (i.e. malloc) failed
    {
        // Standard realloc does NOT free the old pointer if reallocation fails.
        // GNL's typical usage (s1 = ft_realloc(s1,...)) can lead to leaks if NULL is returned
        // and not handled by caller. This implementation follows standard behavior: don't free s1.
        return (NULL);
    }

    // Copy old content. ft_strlcpy needs total size of new_ptr buffer.
    // We copy old_len characters. new_ptr is allocated for new_total_len + 1.
    // ft_strlcpy ensures null termination.
    if (old_len > 0) {
        ft_strlcpy(new_ptr, s1, old_len + 1);
    }
    // The rest of new_ptr (beyond old_len up to new_total_len) is already zeroed by local_calloc.

    free(s1); // Free the old pointer as per realloc behavior on success
    return (new_ptr);
}

int	ft_copynl(char *extra, int baits, char *string, int aux)
{
	int	j;

	j = 0;
	while (extra[j] != '\n' && j < baits)
	{
		string[aux] = extra[j];
		j++;
		aux++;
	}
	return (aux);
}

int	leer(int fd, char *extra)
{
	int	baits;

	baits = read(fd, extra, BUFFER_SIZE);
	if (baits >= 0) // Only null-terminate if read was not an error
	    extra[baits] = '\0';
	// If baits < 0 (error), extra is not modified here; GNL should handle error.
	return (baits);
}
