# libnss-pool

__This version of libnss-pool plugin is not production quality__. It was developed only for joy, just to proove the possibility of the username and uid/gid assignment on getpwnam call. For the potential use case description please check my blog post.

The general idea behind the pool service is to use uid's from a predefined pool as a precofigured set of accounts with yet unknown names, so up to the number of preconfigured accounts `getpwnam()` calls, happening as a result of `getent passwd username` or`su` will not fail but will return `struct passwd` results matching the name provided as `getpwnam` argument with the uid,gid,gecos,dir,shell details preconfigured for the used pool. 
