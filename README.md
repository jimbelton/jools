# jools
Useful utilities. For starters, fang (find and grep).

## fang (find and grep)
This is the beginning of a new implementation in python. It's free software. So far, here's what it can do:

> `usage: fang [-h] [-e` *extension* `| -f` *file-pat*`] [-N] [`*directory*`] [`*pattern*]`
> 
> List the files in a directory (default .), or the lines of files matching a pattern if specified


### positional arguments

| argument  | description |
| --------- | ----------- |
| *directory* | directory to list or search (defaults to '.') |
| *pattern*   | pattern to search for (list files by default) |

### optional arguments
| short option | long option | description |
| ------------ | ----------- | ----------- |
| -h           | --help      | show this help message and exit |
| -e *extension* | --extension *extension* | file names must end in the string '*extension*' |
| -f *file-pat*  | --file-pat *file-pat*   | file name must match the regular expression /*file-pat*/ |
| -N           | --no-line-numbers     | don't include line numbers in output                   |
