--
-- PostgreSQL database dump
--

-- Dumped from database version 16.2 (Debian 16.2-1.pgdg120+2)
-- Dumped by pg_dump version 16.1

-- Started on 2024-03-12 23:01:26 UTC

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- TOC entry 217 (class 1259 OID 16781)
-- Name: msg; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.msg (
                            id integer NOT NULL,
                            user_name character varying(50) NOT NULL,
                            "time" timestamp without time zone NOT NULL,
                            content character varying(500) NOT NULL
);


ALTER TABLE public.msg OWNER TO postgres;

--
-- TOC entry 216 (class 1259 OID 16780)
-- Name: msg_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.msg_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.msg_id_seq OWNER TO postgres;

--
-- TOC entry 3364 (class 0 OID 0)
-- Dependencies: 216
-- Name: msg_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.msg_id_seq OWNED BY public.msg.id;


--
-- TOC entry 215 (class 1259 OID 16773)
-- Name: users; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.users (
                              id character varying(50) NOT NULL,
                              pwd character varying NOT NULL
);


ALTER TABLE public.users OWNER TO postgres;

--
-- TOC entry 3207 (class 2604 OID 16784)
-- Name: msg id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.msg ALTER COLUMN id SET DEFAULT nextval('public.msg_id_seq'::regclass);


--
-- TOC entry 3358 (class 0 OID 16781)
-- Dependencies: 217
-- Data for Name: msg; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.msg VALUES (1, 'bob2', '1999-01-08 04:05:06', 'some text');
INSERT INTO public.msg VALUES (2, 'niels1', '1999-01-08 04:05:01', 'some more text');
INSERT INTO public.msg VALUES (0, 'niels1', '2024-03-12 23:31:53.387', '1234');
INSERT INTO public.msg VALUES (3, 'niels1', '2024-03-12 23:38:08.36', '234');
INSERT INTO public.msg VALUES (4, 'niels1', '2024-03-12 23:38:11.449', '432');
INSERT INTO public.msg VALUES (5, 'niels1', '2024-03-12 23:38:13.231', '523523');
INSERT INTO public.msg VALUES (6, 'niels1', '2024-03-12 23:38:15.07', 'LOL');
INSERT INTO public.msg VALUES (7, 'niels1', '2024-03-12 23:38:17.933', 'AOKSFIJ');
INSERT INTO public.msg VALUES (8, 'niels1', '2024-03-12 23:38:20.11', 'gedfraklmo');
INSERT INTO public.msg VALUES (9, 'niels1', '2024-03-12 23:38:24.425', 'f');
INSERT INTO public.msg VALUES (10, 'niels1', '2024-03-12 23:38:25.381', 'f');
INSERT INTO public.msg VALUES (11, 'niels1', '2024-03-12 23:38:26.29', 'f');
INSERT INTO public.msg VALUES (12, 'niels1', '2024-03-12 23:38:28.111', 'f');
INSERT INTO public.msg VALUES (13, 'niels1', '2024-03-12 23:38:29.727', 'f');
INSERT INTO public.msg VALUES (14, 'niels1', '2024-03-12 23:38:31.497', 'f');
INSERT INTO public.msg VALUES (15, 'niels1', '2024-03-12 23:38:32.965', 'f');
INSERT INTO public.msg VALUES (16, 'niels1', '2024-03-12 23:38:34.985', 'f');
INSERT INTO public.msg VALUES (17, 'niels1', '2024-03-12 23:38:36.896', 'a');
INSERT INTO public.msg VALUES (18, 'niels1', '2024-03-12 23:38:40.329', 'asdadsadsdasadsadsadsadsd');
INSERT INTO public.msg VALUES (19, 'niels1', '2024-03-12 23:38:45.23', '432');
INSERT INTO public.msg VALUES (20, 'niels1', '2024-03-12 23:38:47.241', '432');
INSERT INTO public.msg VALUES (21, 'niels1', '2024-03-12 23:38:48.661', '432');
INSERT INTO public.msg VALUES (22, 'niels1', '2024-03-12 23:38:50.82', '432');
INSERT INTO public.msg VALUES (23, 'niels1', '2024-03-12 23:39:28.102', '432');
INSERT INTO public.msg VALUES (24, 'niels1', '2024-03-12 23:40:52.762', 'mnew');
INSERT INTO public.msg VALUES (25, 'niels1', '2024-03-12 23:42:47.276', 'mnew');
INSERT INTO public.msg VALUES (26, 'niels1', '2024-03-12 23:44:19.048', 'mnew');
INSERT INTO public.msg VALUES (27, 'niels1', '2024-03-12 23:46:12.729', 'fffff');
INSERT INTO public.msg VALUES (28, 'niels1', '2024-03-12 23:47:14.478', '3');
INSERT INTO public.msg VALUES (29, 'niels1', '2024-03-12 23:51:52.992', '453');
INSERT INTO public.msg VALUES (30, 'niels1', '2024-03-12 23:51:54.453', '543');
INSERT INTO public.msg VALUES (31, 'niels1', '2024-03-12 23:51:55.869', '222');
INSERT INTO public.msg VALUES (32, 'niels1', '2024-03-12 23:51:59.185', '1');
INSERT INTO public.msg VALUES (33, 'niels1', '2024-03-12 23:52:01.339', 'IODFIOSDF');
INSERT INTO public.msg VALUES (34, 'niels1', '2024-03-12 23:58:59.378', 'here is a message');
INSERT INTO public.msg VALUES (35, 'niels1', '2024-03-12 23:59:39.514', 'as');
INSERT INTO public.msg VALUES (36, 'niels1', '2024-03-13 00:00:17.035', '123311242141');


--
-- TOC entry 3356 (class 0 OID 16773)
-- Dependencies: 215
-- Data for Name: users; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.users VALUES ('niels1', '1234');
INSERT INTO public.users VALUES ('bob2', '1234');


--
-- TOC entry 3365 (class 0 OID 0)
-- Dependencies: 216
-- Name: msg_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.msg_id_seq', 36, true);


--
-- TOC entry 3211 (class 2606 OID 16788)
-- Name: msg msg_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.msg
    ADD CONSTRAINT msg_pkey PRIMARY KEY (id);


--
-- TOC entry 3209 (class 2606 OID 16779)
-- Name: users users_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_pkey PRIMARY KEY (id);


--
-- TOC entry 3212 (class 2606 OID 16789)
-- Name: msg username; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.msg
    ADD CONSTRAINT username FOREIGN KEY (user_name) REFERENCES public.users(id) NOT VALID;


-- Completed on 2024-03-12 23:01:26 UTC

--
-- PostgreSQL database dump complete
--

