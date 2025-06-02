create table clubs (
club_id serial primary key,
name varchar(50) not null,
city varchar(50) not null,
logo_path varchar(255)
);

CREATE TABLE players (
    player_id serial PRIMARY KEY,
    first_name varchar(50),
    last_name varchar(50) NOT NULL,
    birth DATE,
    nationality varchar(50),
	position varchar(3) check (position in ('GK', 'LB', 'CB', 'RB', 'LM', 'CDM', 'CM', 'RM', 'CAM', 'LW', 'ST', 'RW')),
    club_id integer,
    FOREIGN KEY (club_id) REFERENCES clubs(club_id) ON DELETE SET NULL on update cascade
);

CREATE TABLE coaches (
    coach_id serial PRIMARY KEY,
    first_name VARCHAR(50),
    last_name VARCHAR(50) NOT NULL,
    birth DATE,
	job varchar(50) check (job in ('head coach', 'assistant', 'physical coach', 'gk coach', 'tactics coach')),
    club_id integer,
    FOREIGN KEY (club_id) REFERENCES clubs(club_id) ON DELETE SET NULL on update cascade
);

CREATE TABLE tournaments (
    tournament_id serial PRIMARY KEY,
    name VARCHAR(50) NOT NULL,
    year integer not null,
    country VARCHAR(50) default 'international'
);

CREATE TABLE matches (
    match_id serial PRIMARY KEY,
    match_date date NOT NULL,
    home_club_id integer,
    away_club_id integer,
    score VARCHAR(5) check (score ~ '^[0-9]+-[0-9]+$') default '0-0',
    tournament_id integer,
    FOREIGN KEY (home_club_id) REFERENCES clubs(club_id) ON DELETE CASCADE on update cascade,
    FOREIGN KEY (away_club_id) REFERENCES clubs(club_id) ON DELETE CASCADE on update cascade,
    FOREIGN KEY (tournament_id) REFERENCES tournaments(tournament_id)
	ON DELETE CASCADE on update cascade
);


CREATE TABLE player_stats (
    player_match_id serial PRIMARY KEY,
    match_id integer,
    player_id integer,
    goals integer DEFAULT 0,
    assists integer DEFAULT 0,
    minutes_played integer DEFAULT 0,
	starting_lineup boolean default false,
    FOREIGN KEY (match_id) REFERENCES matches(match_id) ON DELETE CASCADE on update cascade,
    FOREIGN KEY (player_id) REFERENCES players(player_id) ON DELETE CASCADE on update cascade
);

INSERT INTO clubs (name, city, logo_path) VALUES 
('Manchester City', 'Manchester', '/logos/manchester_city.png'),
('Liverpool', 'Liverpool', '/logos/liverpool.png'),
('Real Madrid', 'Madrid', '/logos/real_madrid.png'),
('Barcelona', 'Barcelona', '/logos/barcelona.png'),
('Chelsea', 'London', '/logos/chelsea.png'),
('Atletico Madrid', 'Madrid', '/logos/atletico_madrid.png');

INSERT INTO players (first_name, last_name, birth, nationality, position, club_id) VALUES 
('Kevin', 'De Bruyne', '1991-06-28', 'Belgium', 'CM', 1),
('Erling', 'Haaland', '2000-07-21', 'Norway', 'ST', 1),
('Phil', 'Foden', '2000-05-28', 'England', 'LW', 1),
('Mohamed', 'Salah', '1992-06-15', 'Egypt', 'RW', 2),
('Virgil', 'van Dijk', '1991-07-08', 'Netherlands', 'CB', 2),
('Sadio', 'Mane', '1992-04-10', 'Senegal', 'LW', 2),
('Karim', 'Benzema', '1987-12-19', 'France', 'ST', 3),
('Vinicius', 'Jr.', '2000-07-12', 'Brazil', 'LW', 3),
('Luka', 'Modric', '1985-09-09', 'Croatia', 'CM', 3),
('Lionel', 'Messi', '1987-06-24', 'Argentina', 'RW', 4),
('Robert', 'Lewandowski', '1988-08-21', 'Poland', 'ST', 4),
('Frenkie', 'de Jong', '1997-05-12', 'Netherlands', 'CM', 4),
('NGolo', 'Kante', '1991-03-29', 'France', 'CDM', 5),
('Mason', 'Mount', '1999-01-10', 'England', 'CM', 5),
('Raheem', 'Sterling', '1994-12-08', 'England', 'LW', 5),
('Antoine', 'Griezmann', '1991-09-21', 'France', 'LW', 6),
('Jan', 'Oblak', '1993-01-07', 'Slovenia', 'GK', 6),
('Koke', 'Resurrección', '1992-01-08', 'Spain', 'CM', 6);

INSERT INTO coaches (first_name, last_name, birth, job, club_id) VALUES 
('Pep', 'Guardiola', '1971-01-18', 'head coach', 1),
('Juanma', 'Lillo', '1965-11-02', 'assistant', 1),
('Jürgen', 'Klopp', '1967-06-16', 'head coach', 2),
('Pepijn', 'Lijnders', '1983-04-15', 'assistant', 2),
('Carlo', 'Ancelotti', '1959-06-10', 'head coach', 3),
('David', 'Bettoni', '1970-01-01', 'assistant', 3),
('Xavi', 'Hernandez', '1980-01-25', 'head coach', 4),
('Sergi', 'Barjuan', '1971-12-28', 'assistant', 4),
('Graham', 'Potter', '1975-05-20', 'head coach', 5),
('Bruno', 'Saltor', '1980-10-01', 'assistant', 5),
('Diego', 'Simeone', '1970-04-28', 'head coach', 6),
('Nelson', 'Vivas', '1969-01-01', 'assistant', 6);

INSERT INTO tournaments (name, year, country) VALUES 
('Champions League', 2023, default),
('England Premier League', 2023, 'England'),
('La Liga', 2023, 'Spain');

INSERT INTO matches (match_date, home_club_id, away_club_id, score, tournament_id) VALUES 
('2023-09-01', 1, 2, '2-1', 1),
('2023-09-02', 3, 4, '3-2', 1),
('2023-09-03', 5, 6, '1-1', 2),
('2023-09-04', 1, 3, '1-0', 2),
('2023-09-05', 2, 4, '0-2', 3);

INSERT INTO player_stats (match_id, player_id, goals, assists, minutes_played, starting_lineup) VALUES 
(1, 1, 1, 0, 90, true), 
(1, 2, 1, 0, 90, true),
(1, 3, 0, 1, 90, true),
(1, 4, 0, 0, 90, true),
(1, 5, 0, 0, 90, true),
(1, 6, 0, 0, 90, true),

(2, 7, 2, 0, 90, true),
(2, 8, 1, 1, 90, true),
(2, 9, 0, 0, 90, true),
(2, 10, 0, 0, 90, true),
(2, 11, 0, 0, 90, true),
(2, 12, 0, 0, 90, true),

(3, 13, 0, 0, 90, true), 
(3, 14, 1, 0, 90, true), 
(3, 15, 0, 1, 90, true),
(3, 16, 0, 0, 90, true), 
(3, 17, 0, 0, 90, true),
(3, 18, 0, 0, 90, true),

(4, 1, 1, 0, 90, true),
(4, 2, 0, 1, 90, true),
(4, 3, 0, 0, 90, true),
(4, 7, 2, 0, 90, true), 
(4, 8, 0, 0, 90, true),
(4, 9, 0, 0, 90, true),

(5, 4, 1, 0, 90, true), 
(5, 5, 0, 0, 90, true),
(5, 6, 0, 0, 90, true),
(5, 13, 0, 0, 90, true),
(5, 14, 1, 0, 90, true),
(5, 15, 0, 1, 90, true);