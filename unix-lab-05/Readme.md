Вот писателей 4 
Потерянное обновление

Откуда оно следует из монитора Хоара? Читатели не могут читать, пока в очереди есть ожидающие писатели.
Два типа событий и для чего: автоматический сброс у читателией, ручной сброс у писателей.
автоматический сброс происходит потому, что читатели работают параллельно.

Автоматический для can read, Ручной для can write
Писателем нужен ручной сброс так как они работают в монопольном доступе

Mutex и здесь освобождение
ЧТо можно сказать о коде между захватом и освобождением?

Мьютекс обеспечивает монопльный доступ к коду. 

Каким код делает Мьютекс? Неделилимым

Читатели могут читать параллельно, так как они не изменяют ничего

Пока мы не осоодим мьютекс не один другой процесс не может обратиться к этому коду.

Как ручной сброс обеспечивает монопльный доступ: читатели освобождается автоматически, писатели освобождают вручную,
**чтобы следующий писатель мог его захватить и начать писать**