/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 16:26:15 by seayeo            #+#    #+#             */
/*   Updated: 2024/01/13 16:28:41 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*first;
	t_list	*new;
	void	*ptr;

	if (!f || !del)
		return (NULL);
	first = NULL;
	while (lst)
	{
		ptr = f(lst -> content);
		new = ft_lstnew(ptr);
		if (!new)
		{
			del(ptr);
			ft_lstclear(&first, del);
			return (NULL);
		}
		ft_lstadd_back(&first, new);
		lst = lst -> next;
	}
	lst = NULL;
	return (first);
}
