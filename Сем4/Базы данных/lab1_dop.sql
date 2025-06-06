Create table t1(
num_otdel integer primary key generated by default as identity,
name varchar(10) unique,
prim text,
year_open numeric(4) not null check (year_open between 2000 and 2018)
);

ALTER TABLE t1 
ALTER COLUMN prim 
SET DATA TYPE numeric(5) 
USING prim::numeric(5);

Create table t2(
Tab_num integer primary key generated by default as identity (increment 10),
Fio text,
Dol text
);

Alter table t2 add num_otdel integer;
Alter table t2 add constraint fk foreign key (num_otdel) references t1(num_otdel) on delete set null on update cascade;

Insert into t1 (name, prim, year_open)
Values
('Xxc', 123, 2001),
('Asdfg', 44, 2005);

Insert into t2(fio, dol, num_otdel)
Values
('Fthd', 'sfghdf', 1),
('Sigs', 'gndtynfn', 2);

Insert into t2(fio, dol, num_otdel)
Values
('Ftfgsfdghd', 'sfghdsfgdfgdf', 1);

Update t2 set fio='she' where dol='sfghdf';

Delete from t2 where fio='dhrth';
Delete from t1 where name='dsfg';

select * from t1;

select * from t2;



-- Insert into t1 (name, prim, year_open)
-- Values
-- ('Xxc', 55, 2011);

-- Insert into t1 (name, prim, year_open)
-- Values
-- ('zxc', 55, null);

-- Insert into t1 (name, prim, year_open)
-- Values
-- ('zxc', 55, 2021);
