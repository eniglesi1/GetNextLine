/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eiglesia <eiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 12:47:28 by eniglesi          #+#    #+#             */
/*   Updated: 2025/06/21 00:13:10 by eiglesia         ###   ########.fr       */
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
	int		i;

	i = 0;
	while (s1[i] != '\0')
		i++;
	i += j;
	if (i == 0)
		return (free(s1), NULL);
	a = ft_calloc(sizeof(char), i);
	if (!a)
		return (free(s1), NULL);
	ft_strlcpy(a, s1, (i - j) + 1);
	free(s1);
	return (a);
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

	if (fd < 0 || fd > FOPEN_MAX)
		return (0);
	baits = read(fd, extra, BUFFER_SIZE);
	if (baits != -1)
		extra[baits] = 0;
	else
		extra[0] = 0;
	return (baits);
}
