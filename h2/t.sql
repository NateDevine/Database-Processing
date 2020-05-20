
create table `Supplier` (
  sid integer,
  sname text,
  saddr text
);

create table `Customer` (
  cid integer,
  cname text,
  caddr text
);

create table `Order` (
  oid integer,
  cid integer,
  iid integer,
  quantity integer,
  total float
);

create table `Item` (
  iid integer,
  idescr text,
  price float,
  inven integer
);

create table `Supply` (
  sid integer,
  iid integer,
  cost float
);

insert into `Supplier` values
  (100, "General Widgets", "SD"),
  (101, "Widgitron", "HI"),
  (102, "US Widgets", "ME"),
  (103, "Sycamore Widgets", "IN");

insert into `Customer` values
  (100, "Bob", "IN"),
  (200, "Sally", "IL"),
  (300, "Fred", "OH"),
  (400, "George", "OH"),
  (500, "Sue", "IN"),
  (600, "Jane", "NY");

insert into `Item` values
  (100, "Blue Widget", 1.00, 340),
  (105, "Red Widget", 2.00, 123),
  (109, "Green Widget", 3.00, 13),
  (125, "Orange Widget", 2.00, 50),
  (140, "Orange Widge", 2.00, 50);

insert into `Supply` values
  (100, 100, 0.65),
  (100, 105, 1.65),
  (101, 109, 2.10),
  (101, 125, 1.80),
  (102, 100, 0.66),
  (102, 105, 1.63),
  (103, 125, 1.78);

insert into `Order` values
  (100, 100, 100, 100, 100.00),
  (101, 200, 105,  10,  20.00),
  (102, 100, 105,   5,  10.00),
  (103, 300, 109,   1,   3.00),
  (104, 400, 125,   2,   4.00),
  (105, 500, 125,  30,  60.00),
  (106, 500, 100,  30,  30.00),
  (107, 600, 105,  10,  20.00);
