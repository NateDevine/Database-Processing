-- 1. Find all people who drink 'kona'.

select name
 from Drinks
 where coffee='kona';

-- 2. Find all people who drink 'kona' and visit a shop that serves 'kona'.

select distinct name
 from Drinks natural join Visits natural join Serves
 where coffee='kona';

-- 3. Find all people under the age of 30 who drink 'kona' and visit a shop that
--    serves 'kona'.

select *
 from Drinks natural join Visits natural join Serves natural join People
 where coffee='kona' and age < 30;

-- 4. Find all people under the age of 30 who drink 'kona' and visit a shop that
--    serves 'kona' and where 'kona' costs less than three dollars.

select *
 from Drinks natural join Visits natural join Serves natural join People
 where coffee='kona' and age < 30 and price < 3.0;

-- 5. Find men who do not drink kona.

select name from People where gender='M'
except
select name from Drinks where coffee='kona';

-- 6. Find all (man,woman) pairs who visit the same shop.

select M.name, F.name from
(select * from People natural join Visits where gender='F') as F,
(select * from People natural join Visits where gender='M') as M
where F.shop = M.shop;

-- 7. Find all (man,woman,shop) triples where the man and the woman visit the
--    shop, both drink 'kona', and where 'kona' is served at the shop.

select M.name, F.name, F.shop from
(select * from People natural join Visits natural join Drinks
      where gender='F' and coffee='kona') as F,
(select * from People natural join Visits natural join Drinks
      where gender='M' and coffee='kona') as M
where F.shop = M.shop
  and F.shop in (select shop from Serves where coffee='kona')

-- 8. Find all (man,woman,shop,coffee) quadruples where the man and woman
--    visit the same shop and both drink a coffee served at that shop.

select M.name, F.name, F.shop, F.coffee from
(select * from People natural join Visits natural join Drinks where gender='F') as F,
(select * from People natural join Visits natural join Drinks where gender='M') as M
where F.shop = M.shop and F.coffee = M.coffee
 and F.coffee in (select coffee from Serves where Serves.shop = F.shop)

-- Or maybe (question is ambiguous about what coffee should be):
select M.name, F.name, F.shop, M.coffee, F.coffee from
(select * from People natural join Visits natural join Drinks where gender='F') as F,
(select * from People natural join Visits natural join Drinks where gender='M') as M
where F.shop = M.shop and (
   F.coffee in (select coffee from Serves where Serves.shop = M.shop)
 and
   M.coffee in (select coffee from Serves where Serves.shop = M.shop)
)

 
-- 9. Find all shops that serve a coffee that some person over the age of 80
--    drinks.

select distinct shop from Serves
 where coffee in (select coffee from People natural join Drinks where age > 80)

-- 10. Find all shops visited by a person over the age of 75 who drinks a coffee
--    served at that shop that costs less than four dollars.

select *
from People natural join Drinks natural join Visits natural join Serves
where age > 75 and price < 4
