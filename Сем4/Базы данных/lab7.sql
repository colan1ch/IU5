create or replace function orders_customer_not_exist()
returns trigger as $$
begin
    IF ( TG_OP = 'INSERT' or TG_OP ='UPDATE') then
        IF not exists (select c.cust_id 
                     from customers c
                     where c.cust_id = new.customer_id ) 
        Then 
            Raise exception 'Покупателя с ID % не существует', new.customer_id;
        end if;
    return new;
    end if;
return null;
end;
$$ LANGUAGE plpgsql;
create trigger fk_orders_customer
before insert or update on orders
for each row
execute function orders_customer_not_exist();

insert into orders values
(13, 45, '2024-01-01', null, null, 'C', 130);

update orders set customer_id = 45 where order_id = 1;



create or replace function items_product_not_exist()
returns trigger as $$
begin
    IF ( TG_OP = 'INSERT' or TG_OP ='UPDATE') then
        IF not exists (select p.product_id 
                     from products p
                     where p.product_id = new.product_id ) 
        Then 
            Raise exception 'Товара с ID % не существует', new.product_id;
        end if;
    return new;
    end if;
return null;
end;
$$ LANGUAGE plpgsql;
create trigger fk_items_product
before insert or update on items
for each row
execute function items_product_not_exist();

insert into items values
(15, 2, 40, 4, 120);

update items set product_id = 55 where item_id = 2;



create or replace function orders_cascade_update()
returns trigger as $$
begin 
    update orders
    set customer_id = new.cust_id
    where customer_id = old.cust_id;
    return new;
end;
$$ language plpgsql;
create trigger orders_cascade_upd
after update on customers
for each row 
execute function orders_cascade_update();
--проверка

select * from orders
select * from customers

update customers
set cust_id = 45
where cust_id = 15;



create or replace function orders_cascade_delete()
returns trigger as $$
begin 
    delete from orders
    where customer_id = old.cust_id;
    return old;
end;
$$ LANGUAGE plpgsql;


create trigger orders_cascade_del
after delete on customers
for each row 
execute function orders_cascade_delete();
--проверка

insert into customers (cust_id, last_name, first_name) values
(16, 'test', 'teeeest');
insert into orders values
(13, 16, '2020-01-01', null, null, 'C', 44);

delete from customers
where cust_id = 16;

select * from orders




-- ALTER TABLE items
-- ADD CONSTRAINT fk_items_order
-- FOREIGN KEY (order_id)
-- REFERENCES orders(order_id)
-- ON UPDATE CASCADE
-- ON DELETE CASCADE;

-- ALTER TABLE items
-- ADD CONSTRAINT fk_items_product
-- FOREIGN KEY (product_id)
-- REFERENCES products(product_id)
-- ON UPDATE CASCADE
-- ON DELETE CASCADE;


-- ALTER TABLE orders
-- ADD CONSTRAINT fk_orders_customer
-- FOREIGN KEY (customer_id)
-- REFERENCES customers(cust_id)
-- ON UPDATE CASCADE
-- ON DELETE CASCADE;


-- По названию клиента выдать названия всех товаров, заказанных данным клиентом,
-- с указанием для каждого товара даты последнего заказа.

CREATE OR REPLACE FUNCTION get_customer_products_last_order(c_id INT)
RETURNS TABLE (product_name TEXT, last_order_date DATE) AS $$
DECLARE
    rec record;
    cur CURSOR FOR
        SELECT pr.product_name, MAX(o.order_date) as last_order
        FROM items i
        JOIN products pr ON i.product_id = pr.product_id
        JOIN orders o ON i.order_id = o.order_id
        JOIN customers c ON o.customer_id = c.cust_id
        WHERE c.cust_id = c_id
        GROUP BY pr.product_name;
BEGIN
    OPEN cur;
    LOOP
        FETCH cur INTO rec;
        EXIT WHEN NOT FOUND;
		product_name := rec.product_name;
		last_order_date := rec.last_order;
        RETURN NEXT;
    END LOOP;
    CLOSE cur;
END;
$$ LANGUAGE plpgsql;

SELECT * FROM get_customer_products_last_order(3);



-- 
CREATE OR REPLACE FUNCTION get_product_turnover_by_dates(start_date DATE, end_date DATE)
RETURNS TABLE (product_name TEXT, incoming money) AS $$
DECLARE
    rec RECORD;
BEGIN
    FOR rec IN
        SELECT pr.product_name, SUM(i.quantity * pr.product_price::numeric)::money AS incoming
        FROM items i
        JOIN products pr ON i.product_id = pr.product_id
        JOIN orders o ON i.order_id = o.order_id
        WHERE o.order_date BETWEEN start_date AND end_date
        GROUP BY pr.product_name
    LOOP
        product_name := rec.product_name;
        incoming := rec.incoming;
        RETURN NEXT;
    END LOOP;
    RETURN;
END;
$$ LANGUAGE plpgsql;


select * from get_product_turnover_by_dates('2022-10-10', '2024-10-10')






CREATE OR REPLACE FUNCTION increase_product_prices_by_year(year INT)
RETURNS VOID AS $$
DECLARE
    rec RECORD;
BEGIN
    FOR rec IN
        SELECT o.order_id, o.total_amount
        FROM orders o
        WHERE EXTRACT(YEAR FROM o.order_date) = year
    LOOP
        UPDATE orders o
        SET total_amount = o.total_amount * 1.1
        WHERE order_id = rec.order_id;
    END LOOP;
    RETURN;
END;
$$ LANGUAGE plpgsql;

select * from orders
select increase_product_prices_by_year(2023)



CREATE OR REPLACE FUNCTION get_customer_operations_with_bonus(cust_last_name TEXT)
RETURNS TABLE (operation_date DATE, total MONEY, bonus MONEY) AS $$
DECLARE
    rec RECORD;
    row_number INT := 0;
BEGIN
    FOR rec IN
        SELECT o.order_date, o.total_amount
        FROM orders o
        JOIN customers c ON o.customer_id = c.cust_id
        WHERE c.last_name = cust_last_name
        ORDER BY o.order_date
    LOOP
        row_number := row_number + 1;

        IF row_number % 3 = 0 THEN
            bonus := rec.total_amount * 0.05; 
        ELSE
            bonus := 0; 
        END IF;

        operation_date := rec.order_date;
        total := rec.total_amount;
        RETURN NEXT;
    END LOOP;
    RETURN;
END;
$$ LANGUAGE plpgsql;



select * from get_customer_operations_with_bonus('Иванов')

select * from orders

insert into orders values
(11, 1, '2024-10-10', '2024-11-11', '2024-11-12', 'P', 123),
(12, 1, '2024-10-10', '2024-11-11', '2024-11-12', 'P', 1230)


CREATE OR REPLACE FUNCTION get_product_totals(
    start_date DATE, 
    end_date DATE
)
RETURNS TABLE (
    product_name TEXT,
    operation_date DATE,
    quantity INT,
    total INT
) AS $$
DECLARE
    current_product TEXT := NULL;
    current_total INT := 0;
    rec RECORD;
BEGIN
    FOR rec IN 
        SELECT pr.product_name, o.order_date AS operation_date, i.quantity
        FROM items i
        JOIN products pr ON i.product_id = pr.product_id
        JOIN orders o ON i.order_id = o.order_id
        WHERE o.order_date BETWEEN start_date AND end_date
        ORDER BY pr.product_name, o.order_date
    LOOP
        IF current_product IS DISTINCT FROM rec.product_name THEN
            current_product := rec.product_name;
            current_total := 0;
        END IF;

        current_total := current_total + rec.quantity;

        product_name := rec.product_name;
        operation_date := rec.operation_date;
        quantity := rec.quantity;
        total := current_total;

        RETURN NEXT;
    END LOOP;

    RETURN;
END;
$$ LANGUAGE plpgsql;




select * from get_product_totals('2023-01-01', '2024-01-01')






