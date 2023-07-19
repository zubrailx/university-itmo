# Подготовка окружения (контейнеров)

Create docker image

```
docker build -f=postgres-node.Dockefile . -t rshd-postgres
```

Создаем новую `bridge` сеть:

```
docker network create rshd-bridge
```

Start postgresql nodes with appropriate names:

```
docker run -d \
    -it \
    --network rshd-bridge \
    --name rshd-node-A \
    rshd-postgres

docker run -d \
    -it \
    --network rshd-bridge \
    --name rshd-node-B \
    rshd-postgres

docker run -d \
    -it \
    --network rshd-bridge \
    --name rshd-node-C \
    rshd-postgres
```

Чтобы остановить контейнеры:

```
docker container stop rshd-node-A rshd-node-B rshd-node-C
```

Просмотр запущенных контейнеров:

```
nikit@host lab-4 % docker container ls
CONTAINER ID   IMAGE           COMMAND       CREATED          STATUS          PORTS     NAMES
3d015cd66a81   rshd-postgres   "/bin/bash"   46 seconds ago   Up 46 seconds             rshd-node-C
5095f9c23ee6   rshd-postgres   "/bin/bash"   47 seconds ago   Up 46 seconds             rshd-node-B
929bce58e9bd   rshd-postgres   "/bin/bash"   47 seconds ago   Up 47 seconds             rshd-node-A
```

Подключение к запущенному контейнеру:

```
docker exec -it rshd-node-A bash
```

Докер создал виртуальные сети для каждого из запущенных контейнеров и bridge network:

```
ip a
...
1479: br-c5683b270ac7: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default
    link/ether 02:42:e0:d8:7d:6f brd ff:ff:ff:ff:ff:ff
    inet 172.18.0.1/16 brd 172.18.255.255 scope global br-c5683b270ac7
       valid_lft forever preferred_lft forever
    inet6 fe80::42:e0ff:fed8:7d6f/64 scope link
       valid_lft forever preferred_lft forever
1481: vetha5ece1e@if1480: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue master br-c5683b270ac7 state UP group default
    link/ether 6e:69:1a:1a:a4:da brd ff:ff:ff:ff:ff:ff link-netnsid 0
    inet 169.254.81.255/16 brd 169.254.255.255 scope global noprefixroute vetha5ece1e
       valid_lft forever preferred_lft forever
    inet6 fe80::bbb:fc5:80cb:c969/64 scope link
       valid_lft forever preferred_lft forever
    inet6 fe80::6c69:1aff:fe1a:a4da/64 scope link
       valid_lft forever preferred_lft forever
1483: veth75cca55@if1482: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue master br-c5683b270ac7 state UP group default
    link/ether ca:7c:3c:d6:97:01 brd ff:ff:ff:ff:ff:ff link-netnsid 1
    inet 169.254.206.94/16 brd 169.254.255.255 scope global noprefixroute veth75cca55
       valid_lft forever preferred_lft forever
    inet6 fe80::6c6d:ebc9:3a63:426c/64 scope link
       valid_lft forever preferred_lft forever
    inet6 fe80::c87c:3cff:fed6:9701/64 scope link
       valid_lft forever preferred_lft forever
1485: veth6afb738@if1484: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue master br-c5683b270ac7 state UP group default
    link/ether 72:48:fc:05:5e:fd brd ff:ff:ff:ff:ff:ff link-netnsid 2
    inet 169.254.189.101/16 brd 169.254.255.255 scope global noprefixroute veth6afb738
       valid_lft forever preferred_lft forever
    inet6 fe80::ae72:2936:21cd:2834/64 scope link
       valid_lft forever preferred_lft forever
    inet6 fe80::7048:fcff:fe05:5efd/64 scope link
       valid_lft forever preferred_lft forever
```

Как мы можем увидеть, это `bridge` контейнеров. Пример ниже для первого контейнера:

```
root@929bce58e9bd:/app# cat /sys/class/net/eth0/iflink
1481
```

# ping, ssh контейнеров

## Попытка ssh connect сразу через root

В дальнейшем контейнеры пересоздавались, поэтому идентификаторы могут отличаться.

Для каждого контейнера для подключения по `ssh` производим следующее:

Обновляем пароль для каждого контейнера:

```
passwd
```

Пароль: `root`

Редактируем конфигурацию `ssh`:

```
vim /etc/ssh/sshd_config
```

Добавляем:

```
PermitRootLogin yes
```

Перезапускаем сервис `ssh`

```
service ssh restart
```

Пытаемся залогиниться, и вот проблема:

```
ssh root@7d746f949f9
root@c7d746f949f9's password:
Welcome to Ubuntu 22.04.2 LTS (GNU/Linux 6.1.19-gentoo-x86_64 x86_64)

 * Documentation:  https://help.ubuntu.com
 * Management:     https://landscape.canonical.com
 * Support:        https://ubuntu.com/advantage

This system has been minimized by removing packages and content that are
not required on a system that users do not log into.

To restore this content, you can run the 'unminimize' command.
Last login: Sat Jun  3 09:14:15 2023 from 172.18.0.3
Connection to c7d746f949f9 closed.
```

Решить увы не получилось (поотрубал все скрипты bash, все равно не хочет).

## SSH через обычного пользователя в системе, а потом sh root

Фича докера (нигде не нашел). Чтобы подключиться по ssh, необходимо создать пользователя с таким же именем, как и на основной системе:

```
useradd nikit
passwd nikit
```

Теперь мы можем подключиться по ssh с другого узла. Если мы попытаемся через root или другого пользователя, то все не будет работать: высветится приветственное сообщение выше, и сразу же разорвется соединение.

Видимо, докер для этого не предназначен.

Теперь можно прописать `su root` и все.

Таким образом было показано, что `ssh` с докером костыльно работает. 

## Ping

С узла 2 на узел 1:

```
root@43ca6ff25ce5:/app# ping 172.18.0.2
PING 172.18.0.2 (172.18.0.2) 56(84) bytes of data.
64 bytes from 172.18.0.2: icmp_seq=1 ttl=64 time=0.259 ms
64 bytes from 172.18.0.2: icmp_seq=2 ttl=64 time=0.117 ms
64 bytes from 172.18.0.2: icmp_seq=3 ttl=64 time=0.125 ms
64 bytes from 172.18.0.2: icmp_seq=4 ttl=64 time=0.118 ms
```

# Этап 1

На каждом узле указываем следующие параметры:

## rshd-node-A



## rshd-node-B

## rshd-node-C


# Этап 2.1
