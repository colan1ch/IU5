-- Таблица предприятий-клиентов
CREATE TABLE Enterprises (
    enterprise_id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    ownership_type VARCHAR(100) NOT NULL,
    address TEXT,
    phone VARCHAR(50)
);

-- Таблица типов ценных бумаг
CREATE TABLE Securities (
    security_id SERIAL PRIMARY KEY,
    type_name VARCHAR(100) NOT NULL,
    risk_level INT CHECK (risk_level BETWEEN 1 AND 10),
    description TEXT
);

-- Таблица инвестиций
CREATE TABLE Investments (
    investment_id SERIAL PRIMARY KEY,
    enterprise_id INT NOT NULL REFERENCES Enterprises(enterprise_id),
    security_id INT NOT NULL REFERENCES Securities(security_id),
    investment_amount DECIMAL(15, 2) NOT NULL,
    investment_date DATE NOT NULL,
    expected_profitability DECIMAL(5, 2),
    actual_profitability DECIMAL(5, 2)
);

-- Заполнение предприятий-клиентов
INSERT INTO Enterprises (name, ownership_type, address, phone) VALUES
('ОАО "ГазПром Нефть"', 'Акционерное', 'г. Москва, ул. Победы, 1', '+7 (495) 111-22-33'),
('ПАО "Сбербанк"', 'Публичное акционерное', 'г. Москва, ул. Вавилова, 19', '+7 (495) 222-44-55'),
('ООО "Яндекс"', 'Общество с ограниченной ответственностью', 'г. Москва, ул. Льва Толстого, 16', '+7 (495) 333-66-77'),
('АО "РЖД"', 'Акционерное', 'г. Москва, ул. Новая Басманная, 2', '+7 (495) 444-88-99'),
('ООО "Тинькофф"', 'Общество с ограниченной ответственностью', 'г. Москва, 2-я Хуторская ул., 38А', '+7 (495) 555-00-11'),
('ПАО "Лукойл"', 'Публичное акционерное', 'г. Москва, Сретенский бульвар, 11', '+7 (495) 666-22-33'),
('ООО "ВК"', 'Общество с ограниченной ответственностью', 'г. Санкт-Петербург, Невский пр., 70', '+7 (812) 777-44-55');

-- Заполнение типов ценных бумаг
INSERT INTO Securities (type_name, risk_level, description) VALUES
('ОФЗ (гос. облигации)', 2, 'Облигации федерального займа - самые надежные'),
('Облигации голубых фишек', 4, 'Облигации крупнейших российских компаний'),
('Акции голубых фишек', 5, 'Акции Сбербанка, Газпрома, Лукойла и др.'),
('Акции 2-го эшелона', 7, 'Акции перспективных компаний среднего размера'),
('Корпоративные облигации', 6, 'Облигации средних и малых компаний'),
('ETF на акции США', 5, 'Биржевые фонды на американские акции'),
('Венчурные проекты', 9, 'Инвестиции в стартапы и технологические компании');

-- Заполнение инвестиций (реалистичные суммы и доходности)
INSERT INTO Investments (enterprise_id, security_id, investment_amount, investment_date, expected_profitability, actual_profitability) VALUES
(1, 3, 250000000.00, '2024-01-15', 12.5, 14.2),  -- ГазПром в акции голубых фишек
(1, 1, 150000000.00, '2024-02-01', 8.2, 8.1),   -- ГазПром в гос. облигации
(2, 3, 500000000.00, '2024-01-20', 11.7, 13.3), -- Сбербанк в свои же акции
(2, 6, 200000000.00, '2024-02-10', 15.8, 18.9), -- Сбербанк в ETF США
(3, 7, 50000000.00, '2024-01-25', 35.0, 45.5),  -- Яндекс в венчурные проекты
(3, 4, 80000000.00, '2024-02-05', 22.3, 25.8),  -- Яндекс в акции 2-го эшелона
(4, 1, 300000000.00, '2024-01-30', 7.8, 7.9),   -- РЖД в гос. облигации
(4, 2, 180000000.00, '2024-02-15', 9.2, 10.7),  -- РЖД в облигации голубых фишек
(5, 4, 75000000.00, '2024-02-01', 24.5, 28.1),  -- Тинькофф в акции 2-го эшелона
(5, 6, 120000000.00, '2024-02-20', 16.9, 17.2), -- Тинькофф в ETF США
(6, 3, 320000000.00, '2024-01-18', 10.7, 11.1), -- Лукойл в акции голубых фишек
(6, 5, 150000000.00, '2024-02-12', 13.8, 14.5), -- Лукойл в корп. облигации
(7, 7, 30000000.00, '2024-02-08', 40.2, 52.7),  -- ВК в венчурные проекты
(7, 4, 60000000.00, '2024-02-25', 26.4, 29.8),  -- ВК в акции 2-го эшелона
(1, 6, 90000000.00, '2024-03-01', 14.5, NULL),  -- ГазПром в ETF США (новые)
(2, 5, 120000000.00, '2024-03-05', 12.8, NULL), -- Сбербанк в корп. облигации (новые)
(3, 2, 70000000.00, '2024-03-10', 9.5, NULL);   -- Яндекс в облигации голубых фишек (новые)

-- Добавляем ценные бумаги с низким риском и высокой доходностью
INSERT INTO Securities (type_name, risk_level, description) VALUES
('Государственные облигации с льготным налогообложением', 2, 'Специальные ОФЗ с повышенной доходностью'),
('Корпоративные облигации надежных эмитентов', 3, 'Облигации компаний с госучастием'),
('ETF на облигации развивающихся рынков', 4, 'Фонды на гос. облигации развивающихся стран');

-- Добавляем инвестиции с низким риском и высокой доходностью
INSERT INTO Investments (enterprise_id, security_id, investment_amount, investment_date, expected_profitability, actual_profitability) VALUES
(1, 8, 180000000.00, '2024-03-15', 28.5, 32.1),  -- ГазПром в льготные облигации
(2, 8, 220000000.00, '2024-03-20', 26.8, 31.5),  -- Сбербанк в льготные облигации
(3, 9, 95000000.00, '2024-03-25', 24.3, 29.8),   -- Яндекс в корп. облигации
(4, 10, 150000000.00, '2024-04-01', 22.7, 27.4), -- РЖД в ETF облигации
(5, 8, 85000000.00, '2024-04-05', 25.9, 30.2);   -- Тинькофф в льготные облигации


select * from enterprises;

select * from investments;

select e.name, s.type_name, i.investment_amount, i.investment_date, i.expected_profitability, i.actual_profitability from investments i
join enterprises e on e.enterprise_id = i.enterprise_id
join securities s on s.security_id = i.security_id;

select * from securities;


-- Z-образная функция принадлежности (исправленная)
CREATE OR REPLACE FUNCTION z_fig(x NUMERIC, c NUMERIC, d NUMERIC)
RETURNS DOUBLE PRECISION AS $$
BEGIN
    RETURN CASE
        WHEN x IS NULL THEN 0
        WHEN x <= c THEN 1
        WHEN x > c AND x <= d THEN (d - x) / (d - c)
        ELSE 0
    END;
END;
$$ LANGUAGE plpgsql;

-- Трапециевидная функция принадлежности (исправленная)
CREATE OR REPLACE FUNCTION trap(x NUMERIC, a NUMERIC, b NUMERIC, c NUMERIC, d NUMERIC)
RETURNS DOUBLE PRECISION AS $$
BEGIN
    RETURN CASE
        WHEN x IS NULL THEN 0
        WHEN x < a THEN 0
        WHEN x >= a AND x < b THEN (x - a) / (b - a)
        WHEN x >= b AND x <= c THEN 1
        WHEN x > c AND x <= d THEN (d - x) / (d - c)
        ELSE 0
    END;
END;
$$ LANGUAGE plpgsql;

-- S-образная функция принадлежности (исправленная)
CREATE OR REPLACE FUNCTION s_fig(x NUMERIC, a NUMERIC, b NUMERIC)
RETURNS DOUBLE PRECISION AS $$
BEGIN
    RETURN CASE
        WHEN x IS NULL THEN 0
        WHEN x < a THEN 0
        WHEN x >= a AND x <= b THEN (x - a) / (b - a)
        WHEN x > b THEN 1
        ELSE 0
    END;
END;
$$ LANGUAGE plpgsql;

-- Лингвистическая переменная "Уровень риска" (исправленная)
CREATE OR REPLACE FUNCTION f_risk(attrib_value NUMERIC, term VARCHAR(20))
RETURNS DOUBLE PRECISION AS $$
BEGIN
    RETURN CASE term
        WHEN 'низкий' THEN z_fig(attrib_value, 3, 5)
        WHEN 'средний' THEN trap(attrib_value, 3, 4, 6, 7)
        WHEN 'высокий' THEN s_fig(attrib_value, 6, 8)
        ELSE 0
    END;
END;
$$ LANGUAGE plpgsql;

-- Лингвистическая переменная "Сумма инвестиций" (исправленная)
CREATE OR REPLACE FUNCTION f_investment(attrib_value NUMERIC, term VARCHAR(20))
RETURNS DOUBLE PRECISION AS $$
BEGIN
    RETURN CASE term
        WHEN 'небольшая' THEN z_fig(attrib_value, 50000000, 100000000)
        WHEN 'значительная' THEN trap(attrib_value, 80000000, 120000000, 200000000, 250000000)
        WHEN 'крупная' THEN s_fig(attrib_value, 200000000, 300000000)
        ELSE 0
    END;
END;
$$ LANGUAGE plpgsql;

-- Лингвистическая переменная "Доходность инвестиций" (исправленная)
CREATE OR REPLACE FUNCTION f_profitability(attrib_value NUMERIC, term VARCHAR(20))
RETURNS DOUBLE PRECISION AS $$
BEGIN
    RETURN CASE term
        WHEN 'низкая' THEN z_fig(attrib_value, 8, 12)
        WHEN 'умеренная' THEN trap(attrib_value, 10, 15, 25, 30)
        WHEN 'высокая' THEN s_fig(attrib_value, 25, 35)
        ELSE 0
    END;
END;
$$ LANGUAGE plpgsql;


-- Запрос 1: Инвестиции с высоким риском и высокой доходностью
SELECT 
    i.investment_id,
    e.name as enterprise,
    s.type_name as security,
    s.risk_level,
    i.actual_profitability,
    f_risk(s.risk_level, 'высокий') as risk_high,
    f_profitability(i.actual_profitability, 'высокая') as profit_high
FROM Investments i
JOIN Enterprises e ON i.enterprise_id = e.enterprise_id
JOIN Securities s ON i.security_id = s.security_id
WHERE f_risk(s.risk_level, 'высокий') > 0
AND f_profitability(i.actual_profitability, 'высокая') > 0
ORDER BY LEAST(f_risk(s.risk_level, 'высокий'), f_profitability(i.actual_profitability, 'высокая')) DESC;


-- Запрос 2: Крупные инвестиции предприятий
SELECT 
    e.name as enterprise,
    i.investment_amount,
    ROUND(i.investment_amount / 1000000, 1) as amount_mln,
    f_investment(i.investment_amount, 'крупная') as invest_large
FROM Investments i
JOIN Enterprises e ON i.enterprise_id = e.enterprise_id
WHERE f_investment(i.investment_amount, 'крупная') > 0
ORDER BY f_investment(i.investment_amount, 'крупная') DESC;


-- Запрос 3: Инвестиции с низким риском и умеренной или высокой доходностью
SELECT 
    i.investment_id,
    e.name as enterprise,
    s.type_name as security,
    s.risk_level,
    i.actual_profitability,
    f_risk(s.risk_level, 'низкий') as risk_low,
    GREATEST(f_profitability(i.actual_profitability, 'умеренная'),
             f_profitability(i.actual_profitability, 'высокая')) as profit_mid_high
FROM Investments i
JOIN Enterprises e ON i.enterprise_id = e.enterprise_id
JOIN Securities s ON i.security_id = s.security_id
WHERE f_risk(s.risk_level, 'низкий') > 0
AND GREATEST(f_profitability(i.actual_profitability, 'умеренная'),
             f_profitability(i.actual_profitability, 'высокая')) > 0
ORDER BY GREATEST(f_profitability(i.actual_profitability, 'умеренная'),
                  f_profitability(i.actual_profitability, 'высокая')) DESC;


-- Запрос 4: Значительные инвестиции с низкой доходностью (проблемные)
SELECT 
    i.investment_id,
    e.name as enterprise,
    i.investment_amount,
    i.actual_profitability,
    f_investment(i.investment_amount, 'значительная') as invest_signif,
    f_profitability(i.actual_profitability, 'низкая') as profit_low
FROM Investments i
JOIN Enterprises e ON i.enterprise_id = e.enterprise_id
WHERE f_investment(i.investment_amount, 'значительная') > 0
AND f_profitability(i.actual_profitability, 'низкая') > 0
ORDER BY f_profitability(i.actual_profitability, 'низкая') DESC;


-- Запрос 5: Инвестиции с не низким уровнем риска
SELECT 
    i.investment_id,
    e.name as enterprise,
    s.type_name as security,
    s.risk_level,
    (1 - f_risk(s.risk_level, 'низкий')) as risk_not_low
FROM Investments i
JOIN Enterprises e ON i.enterprise_id = e.enterprise_id
JOIN Securities s ON i.security_id = s.security_id
WHERE (1 - f_risk(s.risk_level, 'низкий')) > 0
ORDER BY (1 - f_risk(s.risk_level, 'низкий')) DESC;


-- Запрос 6: Инвестиции с очень высокой доходностью
SELECT 
    i.investment_id,
    e.name as enterprise,
    i.actual_profitability,
    (f_profitability(i.actual_profitability, 'высокая') ^ 2) as profit_very_high
FROM Investments i
JOIN Enterprises e ON i.enterprise_id = e.enterprise_id
WHERE (f_profitability(i.actual_profitability, 'высокая') ^ 2) > 0
ORDER BY (f_profitability(i.actual_profitability, 'высокая') ^ 2) DESC;


-- Запрос 7: Ценные бумаги с более или менее средним уровнем риска
SELECT 
    s.security_id,
    s.type_name,
    s.risk_level,
    SQRT(f_risk(s.risk_level, 'средний')) as risk_more_or_less_medium
FROM Securities s
WHERE SQRT(f_risk(s.risk_level, 'средний')) > 0
ORDER BY SQRT(f_risk(s.risk_level, 'средний')) DESC;


-- Запрос 8: Инвестиции с высоким риском и высокой доходностью (с CI)
SELECT 
    i.investment_id,
    e.name as enterprise,
    s.type_name as security,
    s.risk_level,
    i.actual_profitability,
    f_risk(s.risk_level, 'высокий') as risk_high,
    f_profitability(i.actual_profitability, 'высокая') as profit_high,
    (f_risk(s.risk_level, 'высокий') + f_profitability(i.actual_profitability, 'высокая')) / 2.0 as CI
FROM Investments i
JOIN Enterprises e ON i.enterprise_id = e.enterprise_id
JOIN Securities s ON i.security_id = s.security_id
WHERE (f_risk(s.risk_level, 'высокий') + f_profitability(i.actual_profitability, 'высокая')) > 0
ORDER BY CI DESC;


-- Запрос 9: Инвестиции с не низким риском и не небольшой суммой (с CI)
SELECT 
    i.investment_id,
    e.name as enterprise,
    s.risk_level,
    i.investment_amount,
    (1 - f_risk(s.risk_level, 'низкий')) as risk_not_low,
    (1 - f_investment(i.investment_amount, 'небольшая')) as invest_not_small,
    ((1 - f_risk(s.risk_level, 'низкий')) + (1 - f_investment(i.investment_amount, 'небольшая'))) / 2.0 as CI
FROM Investments i
JOIN Enterprises e ON i.enterprise_id = e.enterprise_id
JOIN Securities s ON i.security_id = s.security_id
WHERE ((1 - f_risk(s.risk_level, 'низкий')) + (1 - f_investment(i.investment_amount, 'небольшая'))) > 0
ORDER BY CI DESC;


-- Запрос 10: Инвестиции с очень значительной суммой и не низкой доходностью (с CI)
SELECT 
    i.investment_id,
    e.name as enterprise,
    i.investment_amount,
    i.actual_profitability,
    (f_investment(i.investment_amount, 'значительная') ^ 2) as invest_very_signif,
    (1 - f_profitability(i.actual_profitability, 'низкая')) as profit_not_low,
    ((f_investment(i.investment_amount, 'значительная') ^ 2) + (1 - f_profitability(i.actual_profitability, 'низкая'))) / 2.0 as CI
FROM Investments i
JOIN Enterprises e ON i.enterprise_id = e.enterprise_id
WHERE ((f_investment(i.investment_amount, 'значительная') ^ 2) + (1 - f_profitability(i.actual_profitability, 'низкая'))) > 0
ORDER BY CI DESC;



-- Запрос 11: Успешные инвестиции (высокая доходность ИЛИ низкий риск при значительной сумме) с CI
SELECT 
    i.investment_id,
    e.name as enterprise,
    s.type_name as security,
    -- Условие 1: Высокая доходность
    f_profitability(i.actual_profitability, 'высокая') as cond1,
    -- Условие 2: Низкий риск И значительная сумма
    LEAST(f_risk(s.risk_level, 'низкий'), f_investment(i.investment_amount, 'значительная')) as cond2,
    -- CI по правилу OR (берется максимальное значение из условий)
    GREATEST(f_profitability(i.actual_profitability, 'высокая'),
             LEAST(f_risk(s.risk_level, 'низкий'), f_investment(i.investment_amount, 'значительная'))) as CI
FROM Investments i
JOIN Enterprises e ON i.enterprise_id = e.enterprise_id
JOIN Securities s ON i.security_id = s.security_id
WHERE GREATEST(f_profitability(i.actual_profitability, 'высокая'),
               LEAST(f_risk(s.risk_level, 'низкий'), f_investment(i.investment_amount, 'значительная'))) > 0
ORDER BY CI DESC;



-- Запрос 12: Идеальные инвестиции (низкий риск И высокая доходность) с CI
SELECT 
    i.investment_id,
    e.name as enterprise,
    s.type_name as security,
    s.risk_level,
    i.actual_profitability,
    f_risk(s.risk_level, 'низкий') as risk_low,
    f_profitability(i.actual_profitability, 'высокая') as profit_high,
    LEAST(f_risk(s.risk_level, 'низкий'), f_profitability(i.actual_profitability, 'высокая')) as CI
FROM Investments i
JOIN Enterprises e ON i.enterprise_id = e.enterprise_id
JOIN Securities s ON i.security_id = s.security_id
WHERE LEAST(f_risk(s.risk_level, 'низкий'), f_profitability(i.actual_profitability, 'высокая')) > 0
ORDER BY CI DESC;
