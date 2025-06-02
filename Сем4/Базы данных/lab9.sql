CREATE TABLE accounts (
    id SERIAL PRIMARY KEY,
    balance NUMERIC(12, 2) NOT NULL
);

CREATE TABLE transactions (
    id SERIAL PRIMARY KEY,
    from_account_id INTEGER REFERENCES accounts(id),
    to_account_id INTEGER REFERENCES accounts(id),
    amount NUMERIC(12, 2) NOT NULL,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE reservations (
    id SERIAL PRIMARY KEY,
    account_id INTEGER REFERENCES accounts(id),
    amount NUMERIC(12, 2) NOT NULL,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

INSERT INTO accounts (balance) VALUES
(15.50),
(320.25),
(555.55),
(0.00),
(9999.99);


-- Транзакции с несколькими операциями и savepoint
BEGIN;  

UPDATE accounts
SET balance = balance - 200.00
WHERE id = 1;

UPDATE accounts
SET balance = balance + 200.00
WHERE id = 2;

SAVEPOINT before_transaction;

INSERT INTO transactions (from_account_id, to_account_id, amount)
VALUES (1, 2, 200.00);

SELECT * FROM transactions

ROLLBACK TO SAVEPOINT before_transaction;

COMMIT;

-- Блокировка данных (требуется создать второй сеанс)

BEGIN;

SELECT * FROM accounts WHERE id = 1 FOR UPDATE;

COMMIT;

BEGIN TRANSACTION ISOLATION LEVEL READ COMMITTED;

SELECT balance FROM accounts WHERE id = 1;

COMMIT;

-- Бронирование билетов
BEGIN;

SELECT SUM(amount) FROM reservations WHERE timestamp::date = CURRENT_DATE;

INSERT INTO reservations (account_id, amount) VALUES (1, 2);
UPDATE accounts SET balance = balance - 200 WHERE id = 1;
SELECT * FROM transactions;
SELECT * FROM accounts;
COMMIT;

BEGIN TRANSACTION ISOLATION LEVEL READ COMMITTED;

SELECT SUM(amount) FROM reservations WHERE timestamp::date = CURRENT_DATE;

INSERT INTO reservations (account_id, amount) VALUES (1, 2);

COMMIT;

-- Процедура
CREATE OR REPLACE PROCEDURE reserve_tickets(
    p_account_id INT,
    p_amount INT,
    p_ticket_price NUMERIC
)
LANGUAGE plpgsql
AS $$
DECLARE
    total_reserved INT;
    total_capacity INT := 10;
    total_cost NUMERIC := p_amount * p_ticket_price;
BEGIN

    -- Блокируем все текущие бронирования на сегодня
    PERFORM 1 FROM reservations
    WHERE timestamp::date = CURRENT_DATE
    FOR UPDATE;

    -- Считаем, сколько уже забронировано
    SELECT COALESCE(SUM(amount), 0)
    INTO total_reserved
    FROM reservations
    WHERE timestamp::date = CURRENT_DATE;

    -- Проверка доступности мест
    IF total_reserved + p_amount > total_capacity THEN
        RAISE EXCEPTION 'Недостаточно доступных мест. Осталось: %', total_capacity - total_reserved;
    END IF;

    -- Проверка баланса аккаунта
    IF (SELECT balance FROM accounts WHERE id = p_account_id) < total_cost THEN
        RAISE EXCEPTION 'Недостаточно средств на аккаунте';
    END IF;

	PERFORM pg_sleep(10);

    -- Вставка бронирования
    INSERT INTO reservations (account_id, amount)
    VALUES (p_account_id, p_amount);

    -- Списание средств
    UPDATE accounts
    SET balance = balance - total_cost
    WHERE id = p_account_id;

END;
$$;
COMMIT;

CALL reserve_tickets(3, 2, 100);

select * from reservations
TRUNCATE TABLE reservations RESTART IDENTITY;
select * from accounts

UPDATE accounts
SET balance = balance + 1000;

-- Эксперименты с уровнями изоляции
BEGIN ISOLATION LEVEL READ COMMITTED;

CALL reserve_tickets(1, 2, 100);


COMMIT;

BEGIN ISOLATION LEVEL REPEATABLE READ;

CALL reserve_tickets(1, 2, 100);

COMMIT;

BEGIN ISOLATION LEVEL SERIALIZABLE;

CALL reserve_tickets(1, 2, 100);

COMMIT;

