/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eiglesia <eiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 17:23:08 by eniglesi          #+#    #+#             */
/*   Updated: 2025/06/20 23:17:42 by eiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stddef.h>
# include <stdio.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

char	*get_next_line(int fd);
char	*ft_realloc(char *s1, int i);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
int		ft_copynl(char *extra, int baits, char *string, int aux);
int		leer(int fd, char *extra);
void	*ft_calloc(size_t nmemb, size_t size);

#endif
