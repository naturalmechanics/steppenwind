# steppenwind
Steppenwind Productivity Shell


## Shell for Scientists
Simply save the repo, change the file compile to executable using `chmod +x compile`, and then run the file, using `./compile`. Then run the shell using 

`/path/to/steppenwind --configFilePath /path/to/configfile --no-messagebus-manager`

You can use the file `default` in this repo as a configfile.

The file `userCommands.set` lists a set of commands. Each command, a verb, followed by a ` : ` (single blankspace before and after colon) lists a bash command, arguments separated by ;, no psace in between them. Finally you can also add a comment to the same line using a `#` (hash) symbol.

Each command mentioned in `usercommands.set` can be executed in Steppenwind as a child process. Steppenwind keeps track of the children. In a future version, I will include inter-process messaging. 
