/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xazuaje- <xazuaje-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 16:24:16 by xazuaje-          #+#    #+#             */
/*   Updated: 2024/11/14 10:23:12 by xazuaje-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_INPUT_H
# define PARSE_INPUT_H

# include "../types.h"
# include "../utils/utils.h"
# include <unistd.h>
# include <stdlib.h>

int	parse_input(int argc, char **argv, t_program *program);

#endif
