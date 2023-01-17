SELECT name FROM directors
JOIN movies ON directors.movie_id = movies.id
JOIN people ON directors.person_id = people.id
JOIN ratings ON ratings.movie_id = movies.id
WHERE rating >= 9.0;