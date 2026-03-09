/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_arguments_tool.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aymel-ha <aymel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 04:23:42 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/03/09 09:03:32 by aymel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	yes_space(char c)
{
	return ((c >= 9 && c <= 13) || c == ' ');
}

int	yes_int(char c)
{
	return (c >= '0' && c <= '9');
}

int	ft_atoi(char *str)
{
	int		i;
	int		space_length;
	long	result;

	i = 0;
	space_length = 0;
	result = 0;
	while (str[i] != '\0')
	{
		if (yes_space(str[i]))
			space_length++;
		else if (((result * 10) + (str[i] - '0')) > 2147483647)
			return (-42);
		else if (yes_int(str[i]))
			result += (result * 10) + (str[i] - '0');
		else
			return (-42);
		i++;
	}
	return (result);
}
