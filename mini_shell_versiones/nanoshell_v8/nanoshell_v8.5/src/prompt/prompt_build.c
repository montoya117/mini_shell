
#include "nanoshell.h"

#include "nanoshell.h"

/* Calcula la suma de longitudes de todas las partes.
 * Devuelve 0 si count <= 0.
 */
static size_t parts_total_len(const char **parts, int count)
{
	size_t total;
	size_t i;

	if (count <= 0)
		return (0);
	total = 0;
	i = 0;
	while (i < (size_t)count)
	{
		total += ft_strlen(parts[i]);
		i++;
	}
	return (total);
}

/* Copia todas las partes en el buffer 'out' (debe tener espacio suficiente).
 * Al final escribe el terminador '\0'.
 */
static void parts_copy_to_buf(const char **parts, int count, char *out)
{
	size_t pos;
	size_t i;
	size_t len;

	pos = 0;
	i = 0;
	while (i < (size_t)count)
	{
		len = ft_strlen(parts[i]);
		if (len > 0)
		{
			ft_memcpy(out + pos, parts[i], len);
			pos += len;
		}
		i++;
	}
	out[pos] = '\0';
}

/* Une `count` partes en una única cadena alocada una vez.
 * Devuelve heap (caller free) o NULL en fallo.
 */
char *join_parts(const char **parts, int count)
{
	size_t total;
	char *out;

	if (count <= 0)
		return (ft_strdup(""));
	total = parts_total_len(parts, count);
	out = malloc(total + 1);
	if (!out)
		return (NULL);
	parts_copy_to_buf(parts, count, out);
	return (out);
}
//buclee para ir uniendo todas las partes.


static int  prompt_build_parts(t_data *data, char **userhost_out, char **shortcwd_out)
{
    char    *userhost;
    char    *shortcwd;

    userhost = get_user_host();
    shortcwd = get_short_cwd(data);
    if (!userhost)
        userhost = ft_strdup("?");
    if (!shortcwd)
        shortcwd = ft_strdup("?");
    if (!userhost || !shortcwd)
    {
        free(userhost);
        free(shortcwd);
        return (-1);
    }
    *userhost_out = userhost;
    *shortcwd_out = shortcwd;
    return (0);
}

char    *build_prompt(t_data *data)
{
    char        *userhost;
    char        *shortcwd;
    const char  *parts[8];
    char        *res;

    if (prompt_build_parts(data, &userhost, &shortcwd) == -1)
        return (NULL);
    parts[0] = C;
    parts[1] = userhost;
    parts[2] = RST;
    parts[3] = ":";
    parts[4] = G;
    parts[5] = shortcwd;
    parts[6] = RST;
    parts[7] = "$> ";
    res = join_parts(parts, 8);
    free(userhost);
    free(shortcwd);
    return (res);
}
//C, G, RST vienen del header y son secuencias de escape de color
