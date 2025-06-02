create view get_products as
select product_id, product_name, product_price from products;

alter view get_products rename column product_name to product;

alter view get_products rename column product_price to price;

create or replace view get_products as
select product_id, product_name as product, product_price as price from products
where product_price::numeric > 300;


insert into get_products
values (11, 'Шоколад Аленка', 600::money);

select * from get_products

create or replace view view_check as
select last_name, first_name, company_name, city from customers
where company_name like '%ООО%'
with check option;

insert into view_check
values('Шараев1','Вячеслав1', 'ОАО Яндекс', 'Москва')

drop view view_check

select * from view_from_two_tables


create role Test_creator nologin createrole createdb
set role Test_creator
select current_user
create database d
drop database d
create role test
drop role test


create user user1 with password '1234'
set role user1;
select current_user
create database d

set role postgres
grant Test_creator to user1
set role Test_creator
create database d

create role cant_table with password '1234'
set role postgres
grant update on all tables in schema public to cant_table;
revoke all privileges on all tables in schema public from cant_table;
set role cant_table
create table t()
update items set quantity = 2 where quantity = 73;
insert into items
values (1, 1, 1, 1)
select current_user
SELECT grantee, privilege_type
FROM information_schema.role_table_grants
where table_name = 'items'


set role postgres;
create role can_table createdb;
grant all privileges on schema public to can_table;
set role can_table;
create table t();

create role super_role;
grant all privileges on all tables in schema public to super_role;
set role super_role;
create table tqwe();
set role postgres;
revoke insert on table items from super_role;
set role super_role;
insert into items
values (1, 1, 1, 1)

revoke user1 from Test_creator