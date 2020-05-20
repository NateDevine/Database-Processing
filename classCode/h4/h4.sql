
--1) (1pt) Display a listing of all shops with the minimum, maximum and average
--   age (rounded to 2 decimal places) (shop, min, max, average) of their
--   visitors and order the output by increasing average age.

select shop,min(age) as `min`,max(age) as `max`,round(avg(age),2) as average
 from Visits natural join People
group by shop
order by average

--2) (1pt) Produce a top 10 ranking (from most favored to least) of coffees
--   favored by women.

select coffee
from Drinks natural join People
where gender='F'
group by coffee
order by count(coffee) desc limit 10

--3) (2pts) Insert a new person (you!) into your data base and add yourself to
--   the list that likes 'kona' and 'bombon', and that visits 'the little cafe',
--   the busy bean', and 'the hideout'.

insert into People (name, age, gender) values ('Steve Baker', 49, 'M');
insert into Drinks (name, coffee) values 
  ('Steve Baker', 'kona'), ('Steve Baker','bombon');
insert into Visits (name, shop) values
 ('Steve Baker','the little cafe'), ('Steve Baker','the busy bean'),
 ('Steve Baker','the hideout');

--4) (1pt) Raise the price of all coffees by ten percent (UPDATE).

update serves set price=price*1.1

-- 5) (1pt) Delete all 18 year olds from the list of people (in all tables).

delete from drinks where name in
 (select name from People where age=18)

delete from visits where name in
 (select name from People where age=18)

delete from People where age=18

-- 6) (1pt) Delete 'decaffeinated' from the list of coffees (again, all tables).

delete from drinks where coffee = 'decaffeinated'
delete from serves where coffee = 'decaffeinated'

--7) (1pt) Create a view that shows all the available drinks and a count of the
--   number of people that drink it.  The view should have two columns, coffee
--   and count.

create view count as
  select coffee, count(coffee) as count 
  from Drinks
  group by coffee

--8) (2pts) Create a new table with each person listed once. Show the name and
--   age of the person, the most expensive coffee they drink that is served at a
--   place they visit. So the new table has five columns: name, age, coffee,
--   shop, price.

OK, but probably not complete:
create table X as 
 select name, age, coffee, shop, max(price)
 from people natural join drinks natural join serves natural join visits
 group by name;

+  having max(price);
 
Need to add:
insert into X select name, age, NULL, NULL, NULL
 from People
 where name in (select name from People except
  select distinct name from Drinks natural join Serves natural join Visits)
