# libnss-pool

__This version of libnss-pool plugin is not production quality__. It was developed only for joy, just to proove the possibility of the username and uid/gid assignment on getpwnam call. For the potential use case description please check my blog post.

#What is the functionality of pool service?
The general idea behind the pool service is to use uid's from a predefined pool as a precofigured set of accounts with yet unknown names, so up to the number of preconfigured accounts `getpwnam()` calls, happening as a result of `getent passwd username` or`su` will not fail but will return `struct passwd` results matching the name provided as `getpwnam` argument with the uid,gid,gecos,dir,shell details preconfigured for the used pool. 

#How to use it?
Simply compile like specified for nsswitch libraries:
`gcc -fPIC -Wall -shared -o libnss_pool.so -Wl,-soname,libnss_pool.so libnss_pool.c`

and copy the `.so` file to the location used on your system, for Centos 7 it can be for instance /usr/lib64:
`cp ./libnss_pool.so /usr/lib64/`
and add it to passwd database configuration of nsswitch.conf:
```
[root@localhost ~]# grep passwd /etc/nsswitch.conf
passwd:     files pool  sss
```
