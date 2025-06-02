-- a 
-- select * from customers

-- b 
-- select company_name from customers

-- c 
-- select * from items
-- order by total desc

-- d
-- select * from orders 
-- limit 5

-- e
-- select * from items
-- where total::numeric > 500

-- f
-- select * from items 
-- where total::numeric between 300 and 600

-- g
-- select * from customers
-- where cust_id in (
-- select customer_id from orders
-- where paid_date is not null
-- )

-- h
-- select concat(last_name, ' ', first_name) as full_name, company_name, phone from customers
-- where company_name like '%ООО%'

-- i
-- select * from orders
-- where paid_date is null

-- j
-- select round(avg(total_amount::numeric)) as avg_total_amount from orders

-- k
-- select city, count(cust_id) as customers_in_this_city from customers
-- group by city
-- having count(cust_id) > 1

-- l
-- select o.order_id, o.order_date, o.total_amount, c.last_name, c.phone from orders o
-- inner join customers c on o.customer_id = c.cust_id


-- m
-- select distinct city from customers
-- order by city

-- n
-- select i.item_id, quantity from items i
-- where exists (
-- select product_id from products p
-- where i.product_id = p.product_id and p.in_stock > 100
-- )


-- o
-- select cust_id, company_name, last_name, first_name, 
-- case
-- when last_name like '%а' then 'woman'
-- else 'man'
-- end as sex
-- from customers

-- ВАР 21-25
-- 1
-- select o.order_id, o.total_amount, o.order_date, c.last_name, c.phone, c.address
-- from orders o
-- inner join customers c on o.customer_id = c.cust_id
-- where o.order_date between '2023-10-20' and '2023-10-26'
-- order by o.order_date

-- 2
-- select o.total_amount, o. order_date, c.last_name, c.address, c.phone, c.city 
-- from orders o
-- inner join customers c on o.customer_id = c.cust_id
-- where total_amount is not null
-- order by o.total_amount desc
-- limit 1


select * from customers
where city = any (array['Москва', 'Казань']);

select * from products
where product_price > all (select product_price from products where in_stock > 100)
