-- Keep a log of any SQL queries you execute as you solve the mystery.

-- 1.   search crime scene reports database given details of crime including date and street.
SELECT * FROM crime_scene_reports WHERE month = 7 AND day = 28 AND street = 'Chamberlin Street';

-- 2.   search for interviews
SELECT * FROM interviews WHERE year = 2020 AND month = 7 AND day = 28 AND transcript LIKE '%courthouse%';

    /* returned:
    161|Ruth|2020|7|28|Sometime within ten minutes of the theft, I saw the thief get into a car
        in the courthouse parking lot and drive away. If you have security footage from the courthouse
        parking lot, you might want to look for cars that left the parking lot in that time frame.
    162|Eugene|2020|7|28|I don't know the thief's name, but it was someone I recognized. Earlier this
        morning, before I arrived at the courthouse, I was walking by the ATM on Fifer Street and saw
        the thief there withdrawing some money.
    163|Raymond|2020|7|28|As the thief was leaving the courthouse, they called someone who talked to them
        for less than a minute. In the call, I heard the thief say that they were planning to take the
        earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of
        the phone to purchase the flight ticket. */

-- 3.   find names that correspond to the license plates (cars) that left the courhouse parking lot within 10 mins of crime.
SELECT name FROM people WHERE license_plate IN (SELECT license_plate FROM courthouse_security_logs
    WHERE year = 2020 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = 'exit');
    /*  returned: Patrick, Amber, Elizabeth, Roger, Danielle, Russell, Evelyn, Ernest*/

-- 4.   find people whom withdrawled money from an atm on Fifer Street on the day of the crime.
SELECT name FROM people WHERE id IN (
    SELECT person_id FROM bank_accounts WHERE account_number IN (
        SELECT account_number FROM atm_transactions WHERE year = 2020 AND month = 7 AND day = 28
            AND atm_location = 'Fifer Street' AND transaction_type = 'withdraw'));

    /* returned: Bobby, Elizabeth, Victoria, Madison, Roy, Danielle, Russell, Ernest
    that the culprit must be either Elizabeth, Danielle, Russell, or Ernest. */

-- 5.   find people who made a call for the less than a minute
SELECT name FROM people WHERE phone_number IN (
    SELECT caller FROM phone_calls WHERE year = 2020 AND month = 7 AND day = 28 AND duration < 60);
    /* returned: Bobby,..., Russell, ..., Ernest, ... which implies the culprit is either Russell or Ernest */

-- 6.   search where Russell or Ernest left fiftyville on the 29th:
SELECT name FROM people WHERE passport_number IN (
    SELECT passport_number FROM passengers WHERE flight_id = (
        SELECT flights.id FROM flights JOIN airports ON flights.origin_airport_id = airports.id
            WHERE city = 'Fiftyville' AND year = 2020 AND month = 7 AND day = 29 ORDER BY hour, minute LIMIT 1));

-----------------------------------------------------------------------------------------------------------------------

-- to get the culprit find the intersection using the following query:

SELECT name FROM people WHERE license_plate IN (SELECT license_plate FROM courthouse_security_logs
    WHERE year = 2020 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = 'exit')
INTERSECT
SELECT name FROM people WHERE id IN (
    SELECT person_id FROM bank_accounts WHERE account_number IN (
        SELECT account_number FROM atm_transactions WHERE year = 2020 AND month = 7 AND day = 28
            AND atm_location = 'Fifer Street' AND transaction_type = 'withdraw'))
INTERSECT
SELECT name FROM people WHERE phone_number IN (
    SELECT caller FROM phone_calls WHERE year = 2020 AND month = 7 AND day = 28 AND duration < 60)
INTERSECT
SELECT name FROM people WHERE passport_number IN (
    SELECT passport_number FROM passengers WHERE flight_id = (
        SELECT flights.id FROM flights JOIN airports ON flights.origin_airport_id = airports.id
            WHERE city = 'Fiftyville' AND year = 2020 AND month = 7 AND day = 29 ORDER BY hour, minute LIMIT 1));

-- result: Ernest

-- find the destination city
SELECT city FROM airports WHERE id = (
        SELECT destination_airport_id FROM flights WHERE
        year = 2020 AND month = 7 AND day = 29 ORDER BY hour, minute LIMIT 1)

-- result: London

-- find the accomplice:
SELECT name FROM people WHERE phone_number IN (
    SELECT receiver FROM phone_calls WHERE year = 2020 AND month = 7 AND day = 28 AND duration < 60 AND
        caller = (SELECT phone_number FROM people WHERE name = 'Ernest'));

-- result: Berthold







