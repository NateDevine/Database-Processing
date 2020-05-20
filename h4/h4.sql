--1
select shop, min(age) as 'min', max(age) as 'max' , round(avg(age),2) as 'avg' from visits natural join people group by shop order by avg(age) asc;
--2
select shop, count(shop) as 'count' from people natural join visits where gender = "F" group by shop order by count(shop) desc limit 10;
--3
insert into people (name, age, gender) values ("Nate Devine", 21, "M");
insert into Drinks (name, coffee) values ("Nate Devine", "kona");
insert into Drinks (name, coffee) values ("Nate Devine", "bombon");
insert into visits (name,shop) values ('Nate Devine','The Little Cafe');
insert into visits (name,shop) values ('Nate Devine','The Busy Bean');
insert into visits (name,shop) values ('Nate Devine','The Hideout');
--4
update serves set price = price*1.1;
--5
delete from people where age=18;
delete from serves where age=18;
delete from drinks where age=18;
delete from visits where age=18;
--6
delete from Drinks where coffee="decaffeinated";
delete from serves where coffee="decaffeinated";
--7
select coffee, count(name) as 'count' from people natural join drinks group by coffee;
--8
CREATE TABLE new as 
select name, age, coffee, shop, max(price) from people natural join drinks natural join serves natural join visits group by name;
