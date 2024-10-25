#include <stddef.h>

size_t	ft_strlen(const char *s)
{
	size_t	retu;

	retu = 0;
	while (*s != '\0')
	{
		retu++;
		s++;
	}
	return (retu);
}
