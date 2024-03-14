-- Database: fourthingsplus

CREATE TABLE public.task (
                             task_id integer NOT NULL,
                             name character varying NOT NULL,
                             done boolean DEFAULT false NOT NULL,
                             user_id integer NOT NULL
);

CREATE SEQUENCE public.task_task_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE public.users (
                              user_id integer NOT NULL,
                              user_name character varying(50) NOT NULL,
                              password character varying(50)
);

CREATE SEQUENCE public.users_user_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

ALTER TABLE ONLY public.task ALTER COLUMN task_id SET DEFAULT nextval('public.task_task_id_seq'::regclass);

ALTER TABLE ONLY public.users ALTER COLUMN user_id SET DEFAULT nextval('public.users_user_id_seq'::regclass);

INSERT INTO public.task VALUES (1, 'Se Jons videoer', false, 1);

INSERT INTO public.users VALUES (1, 'jon', '1234');

SELECT pg_catalog.setval('public.task_task_id_seq', 1, true);

SELECT pg_catalog.setval('public.users_user_id_seq', 1, true);

ALTER TABLE ONLY public.task
    ADD CONSTRAINT task_pkey PRIMARY KEY (task_id);

ALTER TABLE ONLY public.users
    ADD CONSTRAINT user_name_unique UNIQUE (user_name);

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_pkey PRIMARY KEY (user_id);

ALTER TABLE ONLY public.task
    ADD CONSTRAINT fk_user_task FOREIGN KEY (user_id) REFERENCES public.users(user_id) NOT VALID;