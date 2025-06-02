-- 1. Скалярная функция 
create function company_members_count(company varchar(20))
returns integer
language plpgsql as
$function$
begin
return (
select count(c.last_name) from customers c
where c.company_name = company
);
end;
$function$;
select company_members_count('ООО "Яндекс"')

select * from customers

-- 2. Табличная функция 
create or replace function public.products_by_price(lowest numeric, highest numeric)
returns table(product_name varchar(150), product_price money)
language plpgsql as 
$function$
begin
return query
select p.product_name, p.product_price from products p
where p.product_price::numeric between lowest and highest;
end;
$function$;
select * from products_by_price(200, 500)



-- 3. Хранимая процедура с запросом (минимум 3 штуки)
CREATE OR REPLACE PROCEDURE show_products_above_avg_price()
LANGUAGE plpgsql
AS $$
DECLARE
    product_row RECORD;
BEGIN
    RAISE NOTICE 'Товары с ценой выше средней:';
    FOR product_row IN 
        SELECT product_name, product_price 
        FROM products 
        WHERE product_price::numeric > (SELECT AVG(product_price::numeric) FROM products)
    LOOP
        RAISE NOTICE 'Название: %, Цена: %', 
            product_row.product_name, 
            product_row.product_price;
    END LOOP;
END;
$$;
CALL show_products_above_avg_price();

CREATE OR REPLACE PROCEDURE show_orders_by_city(city_filter VARCHAR(50))
LANGUAGE plpgsql
AS $$
DECLARE
    order_row RECORD;
BEGIN
    RAISE NOTICE 'Заказы из города "%":', city_filter;
    FOR order_row IN 
        SELECT o.order_id, o.order_date, c.company_name, c.last_name, c.first_name
        FROM orders o
        JOIN customers c ON o.customer_id = c.cust_id
        WHERE c.city = city_filter
    LOOP
        RAISE NOTICE 'ID: %, Дата: %, Компания: %, Заказчик: % %', 
            order_row.order_id, 
            order_row.order_date, 
            order_row.company_name,
			order_row.last_name,
			order_row.first_name;
    END LOOP;
END;
$$;
CALL show_orders_by_city('Москва');

create or replace procedure check_product(product varchar(50))
language plpgsql
as $$
begin
if not exists(
select p.product_name from products p
where p.product_name = product
)
then 
	raise notice 'Продукта "%" не существует', product;
else
	if (select p.in_stock from products p where p.product_name = product) > 0
	then
		raise notice 'Продукт "%" есть в наличии', product;
	else
		raise notice 'Продукта "%" нет в наличии', product;
	end if;
end if;
end;
$$;
call check_product('Вафли "Артек"')



-- 4. Хранимая процедура для перехвата исключений 
create or replace procedure get_product_stock(product_name_input varchar(50))
language plpgsql
as $$
declare
    stock_quantity integer;
begin
    SELECT in_stock INTO stock_quantity
    FROM products 
    WHERE product_name = product_name_input;
    
    IF NOT FOUND THEN
        RAISE EXCEPTION 'Товар "%" не найден.', product_name_input;
    END IF;
    
    raise notice 'Количество товара "%" в наличии: %', product_name_input, stock_quantity;
end;
$$;
call get_product_stock('qwe')



-- 5. Функции row_number(), rank (), dense_rank()
CREATE OR REPLACE FUNCTION my_row_number()
RETURNS TABLE (
    row_num BIGINT,
    product_name VARCHAR(100),
    price money
)
LANGUAGE plpgsql
AS $$
DECLARE
    counter BIGINT := 0;
BEGIN
    FOR product_name, price IN 
        SELECT p.product_name, p.product_price 
        FROM products p 
        ORDER BY p.product_price DESC
    LOOP
        counter := counter + 1;
        row_num := counter;
        RETURN NEXT;
    END LOOP;
END;
$$;
SELECT * FROM my_row_number();

CREATE OR REPLACE FUNCTION my_rank()
RETURNS TABLE (
    rank BIGINT,
    product_name VARCHAR(100),
    price money
)
LANGUAGE plpgsql
AS $$
DECLARE
    prev_price money := -1;
    current_rank BIGINT := 0;
    skip_counter BIGINT := 1;
BEGIN
    FOR product_name, price IN 
        SELECT p.product_name, p.product_price 
        FROM products p 
        ORDER BY p.product_price::numeric DESC
    LOOP
        IF price::numeric <> prev_price::numeric THEN
            current_rank := current_rank + skip_counter;
            skip_counter := 1;
            prev_price := price;
        ELSE
            skip_counter := skip_counter + 1;
        END IF;
        rank := current_rank;
        RETURN NEXT;
    END LOOP;
END;
$$;
SELECT * FROM my_rank();

CREATE OR REPLACE FUNCTION my_dense_rank()
RETURNS TABLE (
    rank BIGINT,
    product_name VARCHAR(100),
    price money
)
LANGUAGE plpgsql
AS $$
DECLARE
    prev_price money := -1;
    current_rank BIGINT := 0;
BEGIN
    FOR product_name, price IN 
        SELECT p.product_name, p.product_price 
        FROM products p 
        ORDER BY p.product_price::numeric DESC
    LOOP
        IF price::numeric <> prev_price::numeric THEN
            current_rank := current_rank + 1;
            prev_price := price;
        END IF;
        rank := current_rank;
        RETURN NEXT;
    END LOOP;
END;
$$;
SELECT * FROM my_dense_rank();

SELECT 
  row_number() OVER (ORDER BY product_price) AS row_num,
  rank() OVER (ORDER BY product_price) AS rank_num,
  dense_rank() OVER (ORDER BY product_price) AS dense_rank_num,
  product_name,
  product_price
FROM products;


-- 6. DDL триггер запрета удаления таблиц
create or replace function prevent_table_delete()
returns event_trigger 
language plpgsql
as $$
begin
    raise EXCEPTION 'Вы не можете удалить таблицу';
end
$$;

CREATE EVENT TRIGGER prevent_ddl
ON DDL_COMMAND_START
WHEN TAG IN ('DROP TABLE')
EXECUTE FUNCTION prevent_table_delete();

create table test();
drop table test



-- 7. DML триггер (обновление, вставка, до, после) 
create or replace function check_price_before_insert_update()
returns trigger 
LANGUAGE plpgsql
as $$
DECLARE 
    v_error_message text;
begin 
    if new.product_price::numeric < 0 then
		raise exception 'Цена не может быть отрицательной';
	end if;
	return new;
end;
$$;
create Trigger check_price_before_insert_update
before insert or update on products
    for each row 
    execute FUNCTION check_price_before_insert_update();
insert into products
values (12, 'zxc', -3, 150, true, 'grhfgfkgj')
update products set product_price = -3 where product_id = 1

create or replace function after_insert_update()
returns trigger
language plpgsql
as $$
begin
	if (TG_OP = 'INSERT') then
		raise notice 'Запись добавлена в таблицу %', tg_table_name;
	end if;
	if (TG_OP = 'UPDATE') then
		raise notice 'Запись в таблице % изменена', tg_table_name;
	end if;
	return new;
end;
$$;

create trigger after_insert_update
after insert or update on products
for each row
execute function after_insert_update();

insert into products
values (12, 'zxc', 3, 150, true, 'grhfgfkgj')

update products set in_stock = 25 where product_id = 1




-- 8. Триггер замещения 
create table deleted_products(
	product_id integer primary key,
	product_name varchar(150)
);

create or replace function instead_of_delete()
returns trigger
language plpgsql
as $$
begin
	insert into deleted_products
	values(old.product_id, old.product);
	delete from products
	where product_id = old.product_id;
	return null;
end;
$$;

create trigger instead_of_delete
instead of delete on get_products
for each row
execute function instead_of_delete();


insert into get_products
values (16, 'zxc', 500)

select * from get_products
delete from get_products where product = 'zxc'
select * from deleted_products