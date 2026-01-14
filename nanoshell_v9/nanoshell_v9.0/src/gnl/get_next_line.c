
#include "nanoshell.h"

static char	*read_and_append(int fd, char *stash)
{
	char	buffer[BUFFER_SIZE + 1];
	char	*temp;
	ssize_t	bytes_read;

	while (!stash || !(ft_strchr(stash, '\n')))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
			return (NULL);
		if (bytes_read == 0)
			break ;
		buffer[bytes_read] = '\0';
		temp = ft_strjoin(stash, buffer);
		if (!temp)
		{
			free(stash);
			return (NULL);
		}
		stash = temp;
	}
	return (stash);
}

static char	*extract_line(char *stash)
{
	char	*line;
	ssize_t	len;

	if (!stash || !stash[0])
		return (NULL);
	if (!ft_strchr(stash, '\n'))
		len = ft_strlen(stash);
	else
		len = (ft_strchr(stash, '\n') - stash) + 1;
	line = ft_substr(stash, 0, len);
	if (!line)
		return (NULL);
	return (line);
}

static char	*update_stash(char *stash)
{
	char	*updated_stash;
	char	*pos;
	ssize_t	start;
	ssize_t	left;

	if (!stash || !stash[0])
		return (NULL);
	if (!ft_strchr(stash, '\n'))
	{
		free(stash);
		return (NULL);
	}
	pos = (ft_strchr(stash, '\n'));
	start = ((pos - stash) + 1);
	left = ft_strlen(stash) - start;
	updated_stash = ft_substr(stash, start, left);
	free(stash);
	if (!updated_stash || updated_stash[0] == '\0')
	{
		free(updated_stash);
		return (NULL);
	}
	return (updated_stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = read_and_append(fd, stash);
	if (!stash)
		return (NULL);
	line = extract_line(stash);
	if (!line)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	if (ft_strchr(stash, '\n'))
		stash = update_stash(stash);
	else
	{
		free(stash);
		stash = NULL;
	}
	return (line);
}