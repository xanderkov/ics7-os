STAT=Ssl
S - процесс назодится в режиме прерываемого сна.
s - процесс является лидером сесии
i - процесс является многопоточным

TTY=? - имя терминал, отвечающего за процесс. ? - нет управляющего
Tpgid=-1 - идентификатор группы процесса, который в настоящий момент владеет терминлом tty, к которому подклчючен данный процесс

PID = pgid = sid - собственный идентификатор
pid - идентификатор сесии sid = идентификатор группы pgid

Uid = 0 - user id. Болешиноство демонов имеет привилегии суперпозьзователя и индетификатор пользователя uid = 0

### Вызов SIGHUP

sudo kill -1 pid