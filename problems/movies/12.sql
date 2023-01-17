SELECT title FROM stars
JOIN movies ON movie_id = movies.id
JOIN people ON person_id = people.id
WHERE name = "Johnny Depp"
INTERSECT
SELECT title FROM stars
JOIN movies ON movie_id = movies.id
JOIN people ON person_id = people.id
WHERE name = "Helena Bonham Carter";