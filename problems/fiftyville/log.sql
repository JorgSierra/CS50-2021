-- Keep a log of any SQL queries you execute as you solve the mystery.
-- To get an idea of the DB schema
.schema
-- Following the recomendations to start looking on the crime_scene_reports
.schema crime_scene_reports
-- Filter the crime_scene_reports by date and street to get description and id
SELECT description, id FROM crime_scene_reports WHERE year = 2020 AND month = 7 AND day = 28 AND street = "Chamberlin Street";
-- ********************************************
-- ********** TOOK PLACE AT 10:15 am **********
-- ********************************************
-- Three witnesses where present -> all of them mentioned the courthouse
-- ... Search for the transcriptions containing courthouse
SELECT transcript, name FROM interviews WHERE year = 2020 AND month = 7 AND day = 28 AND transcript LIKE "%courthouse%";

-- //////////////////////////////////////////////////////////////////////////////////////
-- ///////////////////////////    FIRST WITHNESS.   /////////////////////////////////////
-- //////////////////////////////////////////////////////////////////////////////////////
-- Within 10 min of the theft -> Someone took a car from courthouse parking lot
-- ... Search info on parking lot data
.schema courthouse_security_logs
--    What activity types exist?
SELECT DISTINCT(activity) FROM courthouse_security_logs;
--    entrance | exit
--    Look for license plates that exited between 10:15 am and 10:25 am
--    What is the hour format?
SELECT DISTINCT(hour) FROM courthouse_security_logs;
--    24 Hour format
SELECT minute, license_plate FROM courthouse_security_logs WHERE year = 2020 AND month = 7 AND day = 28 AND
activity = "exit" AND hour = 10 AND minute BETWEEN 15 AND 25;
--    minute | license_plate
--        16 | 5P2BI95
--        18 | 94KL13X
--        18 | 6P58WS2
--        19 | 4328GD8
--        20 | G412CB7
--        21 | L93JTIZ
--        23 | 322W7JE
--        23 | 0NTHK55

-- //////////////////////////////////////////////////////////////////////////////////////
-- ///////////////////////////    SECOND WITHNESS.   ////////////////////////////////////
-- //////////////////////////////////////////////////////////////////////////////////////
-- Search for withdraws on Fifer St. ATM's report before the theft
.schema atm_transactions
--    What transaction types exist?
SELECT DISTINCT(transaction_type) FROM atm_transactions;
--    deposit | withdraw
--    Look for withdraws on the theft date at Fifer st
-- Want to know account number and amount
SELECT amount, account_number FROM atm_transactions WHERE year = 2020 AND month = 7 AND day = 28 AND atm_location LIKE "%fifer%";
--     amount | account_number
--         48 | 28500762
--         20 | 28296815
--         60 | 76054385
--         50 | 49610011
--         80 | 16153065
--         10 | 86363979
--         20 | 25506511
--         30 | 81061156
--         35 | 26013199

-- //////////////////////////////////////////////////////////////////////////////////////
-- ////////////////////////////    THIRD WITHNESS.   ////////////////////////////////////
-- //////////////////////////////////////////////////////////////////////////////////////
-- Within 10 min of the theft -> Call duration < 1 min
.schema phone_calls
-- Filter calls by date and call duration <= 1 (min)
SELECT caller, receiver, duration FROM phone_calls WHERE year = 2020 AND month = 7 AND day = 28 AND duration <= 1;
-- The duration of calls appears to be in seconds so filter by duration <= 60 (s)
SELECT caller, receiver, duration FROM phone_calls WHERE year = 2020 AND month = 7 AND day = 28 AND duration <= 60;
--      caller    |    receiver    | duration
-- (130) 555-0289 | (996) 555-8899 | 51
-- (499) 555-9472 | (892) 555-8872 | 36
-- (367) 555-5533 | (375) 555-8161 | 45
-- (609) 555-5876 | (389) 555-5198 | 60
-- (499) 555-9472 | (717) 555-1342 | 50
-- (286) 555-6063 | (676) 555-6554 | 43
-- (770) 555-1861 | (725) 555-3243 | 49
-- (031) 555-6622 | (910) 555-3251 | 38
-- (826) 555-1652 | (066) 555-9701 | 55
-- (338) 555-6650 | (704) 555-2131 | 54
-- Within 10 min of the theft -> Flight ticket was bought
SELECT id, hour, minute, origin_airport_id, destination_airport_id FROM flights WHERE year = 2020 AND month = 7 AND day = 29;
-- Review airports and its contents
SELECT * FROM airports;
-- id | abbreviation |                 full_name               | city
-- 1  |     ORD      | O'Hare International Airport            | Chicago
-- 2  |     PEK      | Beijing Capital International Airport   | Beijing
-- 3  |     LAX      | Los Angeles International Airport       | Los Angeles
-- 4  |     LHR      | Heathrow Airport                        | London
-- 5  |     DFS      | Dallas/Fort Worth International Airport | Dallas
-- 6  |     BOS      | Logan International Airport             | Boston
-- 7  |     DXB      | Dubai International Airport             | Dubai
-- 8  |     CSF      | Fiftyville Regional Airport             | Fiftyville
-- 9  |     HND      | Tokyo International Airport             | Tokyo
-- 10 |     CDG      | Charles de Gaulle Airport               | Paris
-- 11 |     SFO      | San Francisco International Airport     | San Francisco
-- 12 |     DEL      | Indira Gandhi International Airport     | Delhi
-- get data for Fiftyville airport
SELECT * FROM airports WHERE city = "Fiftyville";
-- 8  |     CSF      | Fiftyville Regional Airport             | Fiftyville
-- Look for flights going ouf Fiftyville the next day of the theft
SELECT flights.id, hour, minute FROM flights
JOIN airports ON flights.origin_airport_id = airports.id
WHERE year = 2020 AND month = 7 AND day = 29
AND city = "Fiftyville";
-- id | hour | minute
-- 18 |  16  |  0
-- 23 |  12  |  15
-- 36 |  8   |  20
-- 43 |  9   |  30
-- 53 |  15  |  20
-- Earliest flight out of Fiftyvile -> One day after the theft
SELECT flights.id, MIN(hour), minute FROM flights
JOIN airports ON flights.origin_airport_id = airports.id
WHERE year = 2020 AND month = 7 AND day = 29
AND city = "Fiftyville";
-- id | MIN(hour) | minute
-- 36 |     8     | 20


-- ###############################################
-- #############   GET SUSPECTS   ################
-- ###############################################

-- => Asociate license plate and suspects
--    minute | license_plate
--        16 | 5P2BI95
--        18 | 94KL13X
--        18 | 6P58WS2
--        19 | 4328GD8
--        20 | G412CB7
--        21 | L93JTIZ
--        23 | 322W7JE
--        23 | 0NTHK55
SELECT name, passport_number FROM people
WHERE license_plate IN
(SELECT license_plate FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day = 28
AND activity = "exit" AND hour = 10
AND minute BETWEEN 15 AND 25);
-- ******************************************************************
-- ** PEOPLE WHO WENT OUT THE PARKING LOT IN THE GIVEN TIME WINDOW **
-- ******************************************************************
--      name  | passport_number
--    Patrick |   2963008352
--     Amber  |   7526138472
--  Elizabeth |   7049073643
--     Roger  |   1695452385
--   Danielle |   8496433585
--    Russell |   3592750733
--    Evelyn  |   8294398571
--    Ernest  |   5773159633

-- => Asociate bank account number and suspects
--     amount | account_number
--         48 | 28500762
--         20 | 28296815
--         60 | 76054385
--         50 | 49610011
--         80 | 16153065
--         10 | 86363979
--         20 | 25506511
--         30 | 81061156
--         35 | 26013199
SELECT name, passport_number FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
WHERE account_number IN
(SELECT account_number FROM atm_transactions
WHERE year = 2020 AND month = 7 AND day = 28
AND atm_location LIKE "%fifer%");
-- ******************************************************************
-- ****** PEOPLE WHO MADE A WITHDRAW IN THE GIVEN TIME WINDOW *******
-- ******************************************************************
--      name  | passport_number
--     Ernest |   5773159633
--     Robert |   8304650265
--    Russell |   3592750733
--      Roy   |   4408372428
--     Bobby  |   9878712108
--   Elizabeth|   7049073643
--   Danielle |   8496433585
--    Madison |   1988161715
--   Victoria |   9586786673

-- => Asociate caller phone number and suspects
--      caller    |    receiver    | duration
-- (130) 555-0289 | (996) 555-8899 | 51
-- (499) 555-9472 | (892) 555-8872 | 36
-- (367) 555-5533 | (375) 555-8161 | 45
-- (609) 555-5876 | (389) 555-5198 | 60
-- (499) 555-9472 | (717) 555-1342 | 50
-- (286) 555-6063 | (676) 555-6554 | 43
-- (770) 555-1861 | (725) 555-3243 | 49
-- (031) 555-6622 | (910) 555-3251 | 38
-- (826) 555-1652 | (066) 555-9701 | 55
-- (338) 555-6650 | (704) 555-2131 | 54
SELECT name, passport_number FROM people
WHERE phone_number IN
(SELECT caller FROM phone_calls
WHERE year = 2020 AND month = 7
AND day = 28 AND duration <= 60);
-- ********************************************************************
-- ****** PEOPLE WHO MADE A PHONE CALL IN THE GIVEN TIME WINDOW *******
-- ********************************************************************
--      name  | passport_number
--     Bobby  |   9878712108
--     Roger  |   1695452385
--   Victoria |   9586786673
--    Madison |   1988161715
--    Russell |   3592750733
--    Evelyn  |   8294398571
--    Kathryn |   6121106406
--     Ernest |   5773159633
--    Kimberly|   9628244268


-- For visualization purposes order results

-- PARKING LOT SUSPECTS BY NAME
SELECT name, passport_number FROM people
WHERE license_plate IN
(SELECT license_plate FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day = 28
AND activity = "exit" AND hour = 10
AND minute BETWEEN 15 AND 25) ORDER BY name;
-- MADE WITHDRAW BY NAME
SELECT name, passport_number FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
WHERE account_number IN
(SELECT account_number FROM atm_transactions
WHERE year = 2020 AND month = 7 AND day = 28
AND atm_location LIKE "%fifer%") ORDER BY name;
-- MADE A PHONE CALL BY NAME
SELECT name, passport_number FROM people
WHERE phone_number IN
(SELECT caller FROM phone_calls
WHERE year = 2020 AND month = 7
AND day = 28 AND duration <= 60) ORDER BY name;
-- ******************************************************************
-- ** PEOPLE WHO WENT OUT THE PARKING LOT IN THE GIVEN TIME WINDOW **
--     name   | passport_number
--  Amber     |   7526138472
--  Danielle  |   8496433585 *
--  Elizabeth |   7049073643 *
--  Ernest    |   5773159633 *
--  Evelyn    |   8294398571
--  Patrick   |   2963008352
--  Roger     |   1695452385
--  Russell   |   3592750733 *
-- ****** PEOPLE WHO MADE A WITHDRAW IN THE GIVEN TIME WINDOW *******
--     name   | passport_number
--  Bobby     |   9878712108
--  Danielle  |   8496433585 *
--  Elizabeth |   7049073643 *
--  Ernest    |   5773159633 *
--  Madison   |   1988161715
--  Robert    |   8304650265
--  Roy       |   4408372428
--  Russell   |   3592750733 *
--  Victoria  |   9586786673
-- ****** PEOPLE WHO MADE A PHONE CALL IN THE GIVEN TIME WINDOW *******
--     name   | passport_number
--  Bobby     |   9878712108
--  Ernest    |   5773159633 *
--  Evelyn    |   8294398571
--  Kathryn   |   6121106406
--  Kimberly  |   9628244268
--  Madison   |   1988161715
--  Roger     |   1695452385
--  Russell   |   3592750733 *
--  Victoria  |   9586786673

-- INTERSECT SUSPECTS BY PASSPORT NUMBER
SELECT name, passport_number FROM people
WHERE license_plate IN
(SELECT license_plate FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day = 28
AND activity = "exit" AND hour = 10
AND minute BETWEEN 15 AND 25)
INTERSECT
SELECT name, passport_number FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
WHERE account_number IN
(SELECT account_number FROM atm_transactions
WHERE year = 2020 AND month = 7 AND day = 28
AND atm_location LIKE "%fifer%")
INTERSECT
SELECT name, passport_number FROM people
WHERE phone_number IN
(SELECT caller FROM phone_calls
WHERE year = 2020 AND month = 7
AND day = 28 AND duration <= 60);

-- ****** PEOPLE WHO,
-- ******   -> WENT OUT THE PARKING LOT
-- ******   -> MADE A WITHDRAW
-- ******   -> MADE A PHONE CALL
-- ****** IN THE GIVEN TIME WINDOW:
--     name   | passport_number
--  Ernest    |   5773159633
--  Russell   |   3592750733

-- => Asociate earliest flight passengers and suspects
SELECT seat, passport_number FROM passengers
WHERE flight_id = 36;
-- **************************************************************************
-- **** PASSENGERS WHO TRAVELED IN THE EARLIEST FLIGHT OUT OF FIFTYVILLE ****
-- **************************************************************************
-- seat | passport_number
--  2A  |   7214083635
--  3B  |   1695452385
--  4A  |   5773159633
--  5C  |   1540955065
--  6C  |   8294398571
--  6D  |   1988161715
--  7A  |   9878712108
--  7B  |   8496433585

-- INTERSECT SUSPECTS ALL EVIDENCE BY PASSPORT NUMBER
SELECT passport_number FROM people
WHERE license_plate IN
(SELECT license_plate FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day = 28
AND activity = "exit" AND hour = 10
AND minute BETWEEN 15 AND 25)
INTERSECT
SELECT passport_number FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
WHERE account_number IN
(SELECT account_number FROM atm_transactions
WHERE year = 2020 AND month = 7 AND day = 28
AND atm_location LIKE "%fifer%")
INTERSECT
SELECT passport_number FROM people
WHERE phone_number IN
(SELECT caller FROM phone_calls
WHERE year = 2020 AND month = 7
AND day = 28 AND duration <= 60)
INTERSECT
SELECT passport_number FROM passengers
WHERE flight_id = 36;

-- #*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/
-- #*/@#*/@#*/@#*/@#*/@#*/@ FINAL SUSPECT #*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@
-- #*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/

--  passport_number
--    5773159633

-- Get ALL data from subject
SELECT * FROM people WHERE passport_number =
(SELECT passport_number FROM people
WHERE license_plate IN
(SELECT license_plate FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day = 28
AND activity = "exit" AND hour = 10
AND minute BETWEEN 15 AND 25)
INTERSECT
SELECT passport_number FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
WHERE account_number IN
(SELECT account_number FROM atm_transactions
WHERE year = 2020 AND month = 7 AND day = 28
AND atm_location LIKE "%fifer%")
INTERSECT
SELECT passport_number FROM people
WHERE phone_number IN
(SELECT caller FROM phone_calls
WHERE year = 2020 AND month = 7
AND day = 28 AND duration <= 60)
INTERSECT
SELECT passport_number FROM passengers
WHERE flight_id = 36);

-- THEIF
-- #*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/
--    id   |  name  |  phone_number  | passport_number | license_plate
--  686048 | Ernest | (367) 555-5533 |    5773159633   |   94KL13X
-- #*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/


-- Get accomplice phone number
SELECT receiver FROM phone_calls
WHERE caller = "(367) 555-5533"
AND year = 2020 AND month = 7 AND day = 28
AND duration <= 60;

--     receiver
--  (375) 555-8161

-- Get ALL data from accomplice
SELECT * FROM people WHERE phone_number =
(SELECT receiver FROM phone_calls
WHERE caller = "(367) 555-5533"
AND year = 2020 AND month = 7 AND day = 28
AND duration <= 60);

-- ACCOMPLICE
-- #*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/
--    id   |  name    |  phone_number  | passport_number | license_plate
--  864400 | Berthold | (375) 555-8161 |                 |    4V16VO0
-- #*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/

-- Get flight destination
SELECT abbreviation, full_name, city FROM flights
JOIN airports ON flights.destination_airport_id = airports.id
WHERE flights.id = 36;

-- FLIGHT DESTINATION
-- #*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/
-- abbreviation |     full_name    |  city
--     LHR      | Heathrow Airport | London
-- #*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/@#*/
