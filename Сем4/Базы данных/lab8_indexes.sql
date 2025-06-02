-- ========== ИСХОДНЫЕ ТАБЛИЦЫ ==========

-- Таблица title.basics.tsv → movies
CREATE TABLE movies (
    tconst text PRIMARY KEY,
    titletype text,
    primarytitle text,
    originaltitle text,
    isadult boolean,
    startyear integer,
    endyear integer,
    runtimeminutes integer,
    genres text
);

-- Таблица title.principals.tsv → principals
CREATE TABLE principals (
    tconst text,
    ordering integer,
    nconst text,
    category text,
    job text,
    characters text
);


-- =====================================
-- ========== ЗАДАНИЕ 1: БЕЗ ИНДЕКСОВ ==========

-- Пример простого запроса без индекса
EXPLAIN ANALYZE
SELECT primarytitle FROM movies WHERE startyear = 2020;

-- Пример JOIN без индексов
EXPLAIN ANALYZE
SELECT m.primarytitle, p.nconst
FROM movies m
JOIN principals p ON m.tconst = p.tconst
WHERE p.category = 'producer';

-- =====================================
-- ========== ЗАДАНИЕ 2: B-TREE ИНДЕКСЫ ==========

-- Индексы для WHERE и JOIN
CREATE INDEX idx_startyear ON movies(startyear);
CREATE INDEX idx_category ON principals(category);
CREATE INDEX idx_tconst_movies ON movies(tconst);
CREATE INDEX idx_tconst_principals ON principals(tconst);

drop INDEX idx_startyear;
drop INDEX idx_category;
drop INDEX idx_tconst_movies;
drop INDEX idx_tconst_principals;

ANALYZE principals;
ANALYZE movies;

-- Повторный запрос (должен использовать Index Scan)
EXPLAIN ANALYZE
SELECT primarytitle FROM movies WHERE startyear = 2020;

EXPLAIN ANALYZE
SELECT m.primarytitle, p.nconst
FROM movies m
JOIN principals p ON m.tconst = p.tconst
WHERE p.category = 'producer';
-- "Planning Time: 2.023 ms"

-- =====================================
-- ========== ЗАДАНИЕ 3: HASH ИНДЕКСЫ ==========

-- Hash индекс по nconst
CREATE INDEX idx_hash_nconst ON principals USING HASH(nconst);
-- Запрос с = (равенство) — должен использовать Hash Index
EXPLAIN ANALYZE
SELECT * FROM principals WHERE nconst = 'nm0000151';

-- =====================================
-- ========== ЗАДАНИЕ 4: GIN ИНДЕКСЫ ==========

-- GIN индекс с trigram для LIKE по genres
CREATE EXTENSION IF NOT EXISTS pg_trgm;

CREATE INDEX idx_gin_genres ON movies USING GIN (genres gin_trgm_ops);

-- LIKE-поиск по жанру
EXPLAIN ANALYZE
SELECT * FROM movies WHERE genres LIKE '%Comedy%';

-- =====================================
-- ========== ЗАДАНИЕ 5: ПРОЧИЕ ИНДЕКСЫ ==========

-- PARTIAL INDEX: фильмы, не относящиеся к взрослым
CREATE INDEX idx_partial_adult ON movies(startyear) WHERE isadult = false;

-- MULTI-COLUMN INDEX: сочетание для WHERE и ORDER BY
CREATE INDEX idx_multi_startyear_genres ON movies(startyear, genres);

-- INCLUDE: дополнительное поле для покрытия запроса
CREATE INDEX idx_startyear_inc_title ON movies(startyear) INCLUDE (primarytitle);

-- =====================================
-- ========== ЗАДАНИЕ 6: ОПТИМИЗАЦИЯ ЗАПРОСОВ ==========

-- WHERE
EXPLAIN ANALYZE
SELECT * FROM movies WHERE startyear = 2015;

-- ORDER BY
CREATE INDEX idx_runtime ON movies(runtimeminutes);

EXPLAIN ANALYZE
SELECT * FROM movies ORDER BY runtimeminutes;

-- GROUP BY
EXPLAIN ANALYZE
SELECT startyear, COUNT(*) FROM movies GROUP BY startyear;

-- JOIN
EXPLAIN ANALYZE
SELECT m.primarytitle, p.nconst
FROM movies m
JOIN principals p ON m.tconst = p.tconst
WHERE p.category = 'producer';

-- =====================================
-- ========== ЗАДАНИЕ 7: АНАЛИЗ ПРОИЗВОДИТЕЛЬНОСТИ ==========

-- Статистика использования индексов
SELECT * FROM pg_stat_user_indexes;

-- Статистика таблиц
SELECT * FROM pg_stat_user_tables;

-- Альтернатива: анализ вручную через сравнение EXPLAIN ANALYZE

-- =====================================
-- ========== КОНЕЦ ФАЙЛА ==========
