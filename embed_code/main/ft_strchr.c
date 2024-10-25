#include <stddef.h>

char	*ft_strchr(const char *str, int c)
{
	size_t	count;

	count = 0;
	while (str[count] != '\0')
	{
		if (str[count] == (char)c)
			return ((char *)str + count);
		count++;
	}
	if (str[count] == (char)c)
		return ((char *)str + count);
	return (0);
}
