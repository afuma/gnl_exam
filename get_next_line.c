
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int ft_strlen(char *str, char end)
{
	int i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] && str[i] != end)
		i++;
	return (i);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char *str;
	int	i;
	int	j;

	if (!s1 && !s2)
		return (NULL);
	if (!s2)
		return (s1);
	if (!(str = (char *)malloc(ft_strlen(s1, '\0') + ft_strlen(s2, '\0') + 1)))
		return (NULL);
	i = 0;
	if (s1)
	{
		while (s1[i])
		{
			str[i] = s1[i];
			i++;
		}
	}
	j = 0;
	if (s2)
	{
		while (s2[j])
		{
			str[i + j] = s2[j];
			j++;
		}
	}
	str[i + j] = '\0';
	if (s1)
		free(s1);
	return (str);
}

int ft_strchr(char *str)
{
	while (*str)
	{
		if (*str == '\n')
			return (1);
		str++;
	}
	return (0);
}

// recuperer la ligne courante avant le '\n
char	*ft_split_1(char *buffer)
{
	int i;
	char *result;

	result = malloc(sizeof(char) * (ft_strlen(buffer, '\n') + 2));
	if (!result)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		result[i] = buffer[i];
		i++;
	}
	if (buffer[i] && buffer[i] == '\n')
	{
		result[i] = '\n';
		i++;
	}
	result[i] = '\0';
	return (result);
}

// recuperer le buffer mis a jour, sans la ligne
char	*ft_split_2(char *buffer)
{
	int i;
	int j;
	char *result;

	result = malloc(sizeof(char) * (ft_strlen(buffer, '\0') - ft_strlen(buffer, '\n') + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] && buffer[i] == '\n')
		i++;
	j = 0;
	while (buffer[i])
	{
		result[j] = buffer[i];
		i++;
		j++;
	}
	result[j] = '\0';
	free(buffer);
	return (result);
}

char	*get_next_line(int fd)
{
	static char	*buf;
	char	*output_buf;
	int			read_size;
	char		line_buf[BUFFER_SIZE + 1];

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
    if (buf && ft_strchr(buf))
	{
		output_buf = ft_split_1(buf);
		buf = ft_split_2(buf);
		return (output_buf);
	}
	while ((read_size = read(fd, line_buf, BUFFER_SIZE)) > 0)
	{
		line_buf[read_size] = '\0';
		buf = ft_strjoin(buf, line_buf);
		if (ft_strchr(buf))
		{
			output_buf = ft_split_1(buf);
			buf = ft_split_2(buf);
			return (output_buf);
		}
	}
	if (read_size < 0)
		return (NULL);
	if (read_size == 0 && buf[0] != '\0')
		return (ft_split_1(buf));
	if (buf)
		free(buf);
	return (NULL);
}

int main(int argc, char **argv)
{
    int fd;
    char *line;

    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return (1);
    }
    if ((fd = open(argv[1], O_RDONLY)) < 0)
    {
        perror("Error opening file");
        return (1);
    }
    while ((line = get_next_line(fd)))
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}

