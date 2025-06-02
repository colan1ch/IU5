-- 2b Функцию для поиска информации по названию компании.
create or replace function company_members(company varchar(30))
returns table(customer_id integer, last_name varchar(20), first_name varchar(20), city varchar(15), address varchar(15))
language plpgsql as
$$
begin
return query
select c.cust_id, c.last_name, c.first_name, c.city, c.address from customers c
where c.company_name = company;
end;
$$;

select * from company_members('ООО "Яндекс"')



-- 2c Функцию для поиска товаров по диапазону цен.
create or replace function products_by_price(lowest numeric, highest numeric)
returns table(product_id integer, product_name varchar(150), product_price money, in_stock integer)
language plpgsql as
$$
begin
return query
select p.product_id, p.product_name, p.product_price, p.in_stock
from products p
where p.product_price::numeric between lowest and highest;
end;
$$;

select * from products_by_price(100, 300)


-- 2d Функцию для поиска заказов по дате заказа и/или диапазону дат заказа,
-- доставки, в зависимости от введенных параметров.
create or replace function show_orders(st_order date, nd_order date default null, st_ship date default null, nd_ship default null)
returns table(order_id integer, order_date date, ship_date date)
language plpgsql as
$$
begin
return query
select o.order_id, o.order_date, o.ship_date from orders
WHERE 
	(st_order is null) or 
	(st_order is not null and nd_order is null and o.order_date = st_order) or 
	(nd_order is not null)
ORDER BY o.order_date;
end
$$









-- 2e.1 Получить сгруппированный по городу список с информацией
-- (Nºзаказа, дата заказа, дата доставки) за интервал времени.
create or replace function orders_by_time(st date, nd date)
returns table (order_id integer, order_date date, ship_date date)
language plpgsql as
$$
begin
return query
select o.order_id, o.order_date, o.ship_date from orders o
where o.order_date between st and nd;
end;
$$;

select * from orders_by_time('2023-10-10', '2024-10-10')



-- 2e.2 Подсчитать количество заказов по городам.
create or replace function orders_count_by_city(city_in varchar(15) default '-')
returns table(city_name varchar(15), orders_count bigint)
language plpgsql as
$$
begin
if city_in = '-' then
return query
select c.city, count(o.order_date) from orders o
join customers c on c.cust_id = o.customer_id
group by c.city;
else
return query
select c.city, count(o.order_date) from orders o
join customers c on c.cust_id = o.customer_id
where c.city = city_in
group by c.city;
end if;
end;
$$;

select * from orders_count_by_city()
select * from orders_count_by_city('Москва')




-- 3 4 5 в допке

-- 6
create or replace function before_delete_products()
returns trigger
language plpgsql as
$$
begin
	delete from items where product_id = old.product_id;
	return old;
end;
$$;

create trigger before_delete_products
before delete on products
for each row
execute function before_delete_products();

select * from items

insert into products
values
(12, 'qwe', 155::money, 14, true, 'dfgsdfj');

insert into items
values
(12, 10, 12, 1, 155::money)

delete from products
where product_id = 12




-- 7







