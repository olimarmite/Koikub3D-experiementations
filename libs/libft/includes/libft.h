/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 16:01:07 by motero            #+#    #+#             */
/*   Updated: 2023/10/17 17:37:04 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <stdio.h>
# include <stddef.h>
# include <string.h>
# include <limits.h>
# include <stdlib.h>
# include <stdint.h>
# include <unistd.h>
# include <stdarg.h>
# include <math.h>

int				ft_atoi(const char *nptr);
void			*ft_calloc(size_t nmemb, size_t size);
size_t			ft_strlen(const char *s);
int				ft_isalpha(int c);
int				ft_isdigit(int c);
int				ft_isalnum(int c);
int				ft_isascii(int c);
int				ft_isprint(int c);
void			*ft_memset(void *s, int c, size_t n);
void			*ft_memcpy(void *restrict dst, const void *restrict src, const size_t size);
void			*ft_memchr(const void *s, int c, size_t n);
int				ft_memcmp(const void *s1, const void *s2, size_t n);
void			*ft_memmove(void *dest, const void *src, size_t n);
int				ft_toupper(int c);
int				ft_tolower(int c);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
void			ft_bzero(void *s, size_t n);
char			*ft_strdup(const char *s);
size_t			ft_strlcpy(char *dest, const char *src, size_t size);
char			*ft_strnstr(const char *big, const char *little, size_t len);
char			*ft_strchr(const char *s, int c);
char			*ft_strrchr(const char *s, int c);
size_t			ft_strlcat(char *dst, const char *src, size_t size);
char			*ft_substr(char const *s, unsigned int start, size_t len);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char			*ft_strtrim(char const *s1, char const *set);
size_t			ft_nbr_words(char const *s, char c);
char			**ft_split(char const *s, char c);
char			*ft_itoa(int n);
void			ft_striteri(char *s, void (*f)(unsigned int, char*));
void			ft_putchar_fd(char c, int fd);
void			ft_putstr_fd(char *s, int fd);
void			ft_putendl_fd(char *s, int fd);
void			ft_putnbr_fd(int n, int fd);
double			ft_atof(const char *nptr);
int				ft_strcmp(const char *s1, const char *s2);
char			*ft_strconcat(char *dest, char *src);
char			*ft_strcpy(char *dest, const char *src);
char			*ft_strcat(char *dest, char *src);

typedef struct s_list
{
	void			*content;
	size_t			content_size;
	struct s_list	*next;
}					t_list;

t_list			*ft_lstnew(void *content);
void			ft_lstadd_front(t_list **lst, t_list *new);
int				ft_lstsize(t_list *lst);
t_list			*ft_lstlast(t_list *lst);
void			ft_lstadd_back(t_list **lst, t_list *new);
void			ft_lstdelone(t_list *lst, void (*del)(void *));
void			ft_lstclear(t_list **lst, void (*del)(void *));
void			ft_lstiter(t_list *lst, void (*f)(void *));
t_list			*ft_lstmap(t_list *lst, void *(*f)(void *),
					void (*del)(void *));

//Main fonction

int				ft_printf(const char *s, ...);
int				ft_flags(va_list args, char c);
// Switch case fonction and length counting
int				ft_single_char(int c);
int				ft_string(char	*s);
int				ft_address(unsigned long ptr);
int				ft_nbr_signed(signed int nbr);
int				ft_nbr_unsigned(char c, long int nbr);
int				ft_percent(void);
char			ft_print_char(char c, const char *str);

//Utils fonctions
void			ft_putchar(char c);
int				ft_print_addr(unsigned long addr);
int				ft_atoi_base(char *str, char *base);
void			ft_putstr(char *str);
void			ft_putnbr_base(long int nbr, char *base);
void			ft_printerror(char *str);
#endif
