/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eiglesia <eiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 21:49:20 by eniglesi          #+#    #+#             */
/*   Updated: 2025/06/20 23:10:08 by eiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

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
	int		n;

	i = 0;
	while (s1[i] != '\0')
		i++;
	i += j;
	if (i == 0)
		return (free(s1), NULL);
	a = malloc(sizeof(char) * i + 1);
	if (!a)
		return (free(s1), NULL);
	n = sizeof(char) * i + 1;
	while (n > 0)
	{
		n--;
		((char *)a)[n] = '\0';
	}
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

	baits = read(fd, extra, BUFFER_SIZE);
	if (baits != -1)
		extra[baits] = 0;
	else
		extra[0] = 0;
	return (baits);
}

int	ft_is_line(char *extra)
{
	int	i;

	i = 0;
	while (extra[i] != '\n' && extra[i])
		i++;
	if (extra[i] == '\n')
		return (i);
	return (-1);
}
