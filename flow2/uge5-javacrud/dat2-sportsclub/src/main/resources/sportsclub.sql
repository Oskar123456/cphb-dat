-- Table structure for table zip
CREATE TABLE zip (
  zip int PRIMARY KEY,
  city varchar(45) NOT NULL
);

-- Dumping data for table zip
INSERT INTO zip (zip, city) VALUES
  (3700, 'Rønne'),
  (3730, 'Nexø'),
  (3740, 'Svanneke'),
  (3760, 'Gudhjem'),
  (3770, 'Allinge'),
  (3782, 'Klemmensker');

-- Table structure for table sport
CREATE TABLE sport (
  sport_id serial PRIMARY KEY,
  sport varchar(45) NOT NULL
);

-- Dumping data for table sport
INSERT INTO sport (sport) VALUES
  ('Yoga'),
  ('Fodbold'),
  ('Håndbold'),
  ('TaiChi'),
  ('Svømning'),
  ('Tennis'),
  ('Bordtennis'),
  ('Gymnastik');

-- Table structure for table team
CREATE TABLE team (
  team_id varchar(10) NOT NULL,
  sport_id int NOT NULL,
  PRIMARY KEY (team_id),
  CONSTRAINT fk_team_sport1 FOREIGN KEY (sport_id) REFERENCES sport (sport_id)
);

-- Dumping data for table team
INSERT INTO team (team_id, sport_id) VALUES
  ('yo01', 1),
  ('yo02', 1),
  ('fo01', 2),
  ('h01', 3),
  ('tai01', 4),
  ('tai02', 4),
  ('sv01', 5),
  ('sv02', 5),
  ('ten01', 6),
  ('ten02', 6),
  ('bt01', 7),
  ('gym01', 8);

-- Table structure for table member
CREATE TABLE member (
  member_id serial PRIMARY KEY,
  name varchar(45),
  address varchar(45),
  zip int,
  gender varchar(1),
  year varchar(45),
  CONSTRAINT fk_member_zip FOREIGN KEY (zip) REFERENCES zip (zip)
);

-- Dumping data for table member
INSERT INTO member (name, address, zip, gender, year) VALUES
  ('Hans Sørensen', 'Agernvej 3', 3700, 'm', '2000'),
  ('Jens Kofoed', 'Agrevej 5', 3700, 'm', '2001'),
  ('Peter Lundin', 'Ahlegårdsvejen 7', 3700, 'm', '2002'),
  ('Frederik Carlsen', 'Ahlstrandsvej 12', 3700, 'm', '2005'),
  ('Peter Petersen', 'Ahornvej 54', 3700, 'm', '1999'),
  ('Ida Kofoed', 'Blekingevej 43', 3700, 'k', '1999'),
  ('Bente Svendsen', 'Blemmelyngvejen 32', 3700, 'k', '1998'),
  ('Tove Jørgensen', 'Blommehaven 2', 3700, 'k', '1995'),
  ('Søren Langeland', 'Danielstræde 4', 3700, 'm', '1996'),
  ('Jacob Sørensen', 'Degnebrovejen 5', 3700, 'm', '1996'),
  ('Nikolaj Kofoed', 'Dittes Vej 1', 3700, 'm', '1970'),
  ('Nynne Sofia', 'Doktorbakken 112', 3700, 'k', '1970'),
  ('Per Jørgensen', 'Doktordammen 76', 3760, 'm', '1980'),
  ('Ole Lukøje', 'Ejnar Jensens Vej 32', 3730, 'm', '1985'),
  ('Rasmus Kofoed', 'Ejnar Mikkelsensvej 6', 3740, 'm', '1990'),
  ('Jens Nielsen', 'Ekkodalsvejen 6', 3740, 'm', '1950'),
  ('Marinus Bastiansen', 'Elisabetsvej 11', 3730, 'm', '1955'),
  ('Jesper Find Nilsen', 'Ellebakken 4', 3760, 'm', '1958'),
  ('Lars Kofoed', 'Ellebyvej 56', 3740, 'm', '1959'),
  ('Søren Olsen', 'Fabriksvej 45', 3770, 'm', '1960'),
  ('Emil Jørgensen', 'Falckvej 7', 3740, 'm', '1954'),
  ('Benjamin Sørensen', 'Falsterbovej 98', 3760, 'm', '1943'),
  ('Rigmor Jensen', 'Fasanvangen 8', 3782, 'k', '2000'),
  ('Ulrikke Kofoed', 'Glappevej 34', 3730, 'k', '2001'),
  ('Karla Svendsen', 'Glastorvet 3', 3782, 'k', '2002'),
  ('Sørine Olsen', 'Glasværksvej 6', 3760, 'k', '2005'),
  ('Magda Boulet', 'Glentevangen 8', 3782, 'k', '1999'),
  ('Viktor Sørensen', 'Godthåbsvej 42', 3782, 'm', '1999'),
  ('Frank Kofoed', 'Hallandsvej 65', 3782, 'm', '1980'),
  ('Karl Nielsen', 'Hallebakken 87', 3760, 'm', '1985'),
  ('Ulrik Pedersen', 'Halledalsvej 74', 3770, 'm', '1990'),
  ('Jeppe Sørensen', 'Hallegårdsvejen 25', 3740, 'm', '1950'),
  ('Mathias Kofoed', 'Helligdomsvej 67', 3700, 'm', '1955'),
  ('Marcus Nielsen', 'Helligpedervej 27', 3770, 'm', '1958'),
  ('Joshua Johansen', 'Helsevej 28', 3700, 'm', '1959'),
  ('Kenneth Sørensen', 'Henrik Hansensgade 29', 3700, 'm', '1980'),
  ('Dorthe Schou', 'Hentregårdsvej 61', 3760, 'k', '1985'),
  ('Dorte Kofoed', 'Jomfrudalen 64', 3700, 'k', '1990'),
  ('Johan Frederiksen', 'Jomfruskoven 3', 3700, 'm', '1950'),
  ('Johanne Nielsen', 'Jomfrustien 14', 3730, 'k', '1955'),
  ('Inger Lundbæk', 'Jons Kapelvej 17', 3700, 'k', '1958'),
  ('Rikke Sørensen', 'Jordbærdalen 6', 3700, 'k', '1959'),
  ('Vibeke Wagener', 'Jordbærhaven 8', 3700, 'k', '2002'),
  ('Charlotte Nielsen', 'Knudsvej 10', 3700, 'k', '2005'),
  ('Mats Edin', 'Kobbevej 62', 3770, 'm', '1999'),
  ('Bjørn Andersen', 'Kodalsvejen 12', 3770, 'm', '1999'),
  ('Jakob Skov', 'Kofoedsvej 7', 3730, 'm', '1998'),
  ('Jønke Svendsen', 'Jomfrudalen 13', 3770, 'm', '1995'),
  ('Svend Svin', 'Jomfruskoven 21', 3770, 'm', '2004'),
  ('Nynne Sofia', 'Doktorbakken 112', 3700, 'k', '1970'),
  ('Nynne Sofia', 'Doktorbakken 112', 3700, 'k', '1970'),
  ('Ole Olsen', 'Banegade 2', 3700, 'm', '1967'),
  ('Ole Olsen', 'Banegade 2', 3700, 'm', '1967');

-- Table structure for table registration
CREATE TABLE registration (
  member_id int NOT NULL,
  team_id varchar(10) NOT NULL,
  price int,
  PRIMARY KEY (member_id, team_id),
  CONSTRAINT fk_registration_member1 FOREIGN KEY (member_id) REFERENCES member (member_id),
  CONSTRAINT fk_registration_team1 FOREIGN KEY (team_id) REFERENCES team (team_id)
);

-- Dumping data for table registration
INSERT INTO registration (member_id, team_id, price) VALUES
  (2, 'yo02', 168),
  (3, 'fo01', 146),
  (4, 'h01', 126),
  (5, 'tai01', 198),
  (6, 'tai02', 130),
  (7, 'sv01', 180),
  (8, 'sv02', 129),
  (9, 'ten01', 119),
  (10, 'ten02', 197),
  (11, 'bt01', 157),
  (12, 'gym01', 128),
  (13, 'yo02', 116),
  (14, 'fo01', 194),
  (15, 'h01', 195),
  (16, 'tai01', 198),
  (17, 'tai02', 157),
  (18, 'sv01', 102),
  (19, 'sv02', 126),
  (20, 'ten01', 105),
  (21, 'ten02', 118),
  (22, 'bt01', 114),
  (23, 'gym01', 179),
  (24, 'yo02', 184),
  (25, 'fo01', 138),
  (26, 'h01', 127),
  (27, 'tai01', 183),
  (28, 'tai02', 153),
  (29, 'sv01', 137),
  (30, 'sv02', 164),
  (31, 'ten01', 163),
  (32, 'ten02', 122),
  (33, 'bt01', 127),
  (34, 'gym01', 113),
  (35, 'yo02', 198),
  (36, 'fo01', 194),
  (37, 'h01', 135),
  (38, 'tai01', 135),
  (39, 'tai02', 119),
  (40, 'sv01', 158),
  (41, 'sv02', 189),
  (42, 'ten01', 181),
  (43, 'ten02', 102),
  (44, 'bt01', 151),
  (45, 'gym01', 174),
  (46, 'yo02', 107),
  (47, 'fo01', 122),
  (48, 'h01', 176),
  (49, 'tai01', 138),
  (50, 'tai02', 121);
