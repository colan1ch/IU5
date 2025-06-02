-- 5
UPDATE accounts SET balance = balance + 100 WHERE id = 1;

-- 6
BEGIN;
WITH locked AS (
    SELECT * FROM reservations
    WHERE timestamp::date = CURRENT_DATE
)
SELECT SUM(amount) AS reserved FROM locked;

INSERT INTO reservations (account_id, amount) VALUES (2, 2);
UPDATE accounts SET balance = balance - 200 WHERE id = 2;
COMMIT;

BEGIN;
SELECT SUM(amount) FROM reservations WHERE timestamp::date = CURRENT_DATE;
INSERT INTO reservations (account_id, amount) VALUES (2, 2);
COMMIT;

CALL reserve_tickets(4, 2, 100);

-- 8
BEGIN ISOLATION LEVEL READ COMMITTED;

CALL reserve_tickets(2, 9, 100);


COMMIT;

BEGIN ISOLATION LEVEL REPEATABLE READ;

CALL reserve_tickets(4, 9, 100);

COMMIT;

BEGIN ISOLATION LEVEL SERIALIZABLE;

CALL reserve_tickets(2, 9, 100);

COMMIT;

