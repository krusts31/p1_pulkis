#include "libft.h"
#include <sys/types.h>

char	*ft_strrchr(const char *str, int c)
{
	ssize_t	count;

	count = ft_strlen(str);
	while (count >= 0)
	{
		if (str[count] == (char)c)
			return ((char *)str + count);
		count--;
	}
	return (NULL);
}
