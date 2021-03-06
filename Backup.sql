PGDMP     .    $        
        x           GestionComptable    11.4    11.4                0    0    ENCODING    ENCODING        SET client_encoding = 'UTF8';
                       false                       0    0 
   STDSTRINGS 
   STDSTRINGS     (   SET standard_conforming_strings = 'on';
                       false                       0    0 
   SEARCHPATH 
   SEARCHPATH     8   SELECT pg_catalog.set_config('search_path', '', false);
                       false                       1262    16393    GestionComptable    DATABASE     �   CREATE DATABASE "GestionComptable" WITH TEMPLATE = template0 ENCODING = 'UTF8' LC_COLLATE = 'French_France.1252' LC_CTYPE = 'French_France.1252';
 "   DROP DATABASE "GestionComptable";
             postgres    false            �            1259    16397    Clients    TABLE     �   CREATE TABLE public."Clients" (
    "Nom" text,
    "Adresse" text,
    "Code Postal" integer,
    "Ville" text,
    "Correspondant" text,
    "N°" integer NOT NULL
);
    DROP TABLE public."Clients";
       public         postgres    false            �            1259    16405 	   Commandes    TABLE     r   CREATE TABLE public."Commandes" (
    "Reference Client" integer,
    "N°" integer NOT NULL,
    "Objet" text
);
    DROP TABLE public."Commandes";
       public         postgres    false            �            1259    16424 *   Facturation Description Produit/Prestation    TABLE     �   CREATE TABLE public."Facturation Description Produit/Prestation" (
    "Facture" integer,
    "Quantite" integer,
    "Remise %" double precision,
    "Description" text,
    "N°" integer NOT NULL,
    "PU brut" double precision
);
 @   DROP TABLE public."Facturation Description Produit/Prestation";
       public         postgres    false            �            1259    16413    Factures    TABLE     U  CREATE TABLE public."Factures" (
    "N°" integer NOT NULL,
    "Commande" integer,
    "Date de Facturation" date,
    "Facture avec TVA" boolean,
    "Date de paiement" date,
    "Description pour Client" text,
    "Encaissement" boolean,
    "Date Echeance" date,
    "Remise globale" double precision,
    "Mode de Paiement" integer
);
    DROP TABLE public."Factures";
       public         postgres    false            �            1259    16435    Mode de Paiement    TABLE     X   CREATE TABLE public."Mode de Paiement" (
    "N°" integer NOT NULL,
    "Mode" text
);
 &   DROP TABLE public."Mode de Paiement";
       public         postgres    false                      0    16397    Clients 
   TABLE DATA               e   COPY public."Clients" ("Nom", "Adresse", "Code Postal", "Ville", "Correspondant", "N°") FROM stdin;
    public       postgres    false    196   J                 0    16405 	   Commandes 
   TABLE DATA               I   COPY public."Commandes" ("Reference Client", "N°", "Objet") FROM stdin;
    public       postgres    false    197   �                 0    16424 *   Facturation Description Produit/Prestation 
   TABLE DATA               �   COPY public."Facturation Description Produit/Prestation" ("Facture", "Quantite", "Remise %", "Description", "N°", "PU brut") FROM stdin;
    public       postgres    false    199   �                 0    16413    Factures 
   TABLE DATA               �   COPY public."Factures" ("N°", "Commande", "Date de Facturation", "Facture avec TVA", "Date de paiement", "Description pour Client", "Encaissement", "Date Echeance", "Remise globale", "Mode de Paiement") FROM stdin;
    public       postgres    false    198   �                 0    16435    Mode de Paiement 
   TABLE DATA               ;   COPY public."Mode de Paiement" ("N°", "Mode") FROM stdin;
    public       postgres    false    200   �       �
           2606    16404    Clients Clients_pkey 
   CONSTRAINT     Y   ALTER TABLE ONLY public."Clients"
    ADD CONSTRAINT "Clients_pkey" PRIMARY KEY ("N°");
 B   ALTER TABLE ONLY public."Clients" DROP CONSTRAINT "Clients_pkey";
       public         postgres    false    196            �
           2606    16431 N   Facturation Description Produit/Prestation Description Produit/Prestation_pkey 
   CONSTRAINT     �   ALTER TABLE ONLY public."Facturation Description Produit/Prestation"
    ADD CONSTRAINT "Description Produit/Prestation_pkey" PRIMARY KEY ("N°");
 |   ALTER TABLE ONLY public."Facturation Description Produit/Prestation" DROP CONSTRAINT "Description Produit/Prestation_pkey";
       public         postgres    false    199            �
           2606    16412    Commandes Factures_pkey 
   CONSTRAINT     \   ALTER TABLE ONLY public."Commandes"
    ADD CONSTRAINT "Factures_pkey" PRIMARY KEY ("N°");
 E   ALTER TABLE ONLY public."Commandes" DROP CONSTRAINT "Factures_pkey";
       public         postgres    false    197            �
           2606    16442 &   Mode de Paiement Mode de Paiement_pkey 
   CONSTRAINT     k   ALTER TABLE ONLY public."Mode de Paiement"
    ADD CONSTRAINT "Mode de Paiement_pkey" PRIMARY KEY ("N°");
 T   ALTER TABLE ONLY public."Mode de Paiement" DROP CONSTRAINT "Mode de Paiement_pkey";
       public         postgres    false    200            �
           2606    16420    Factures N° 
   CONSTRAINT     Q   ALTER TABLE ONLY public."Factures"
    ADD CONSTRAINT "N°" PRIMARY KEY ("N°");
 :   ALTER TABLE ONLY public."Factures" DROP CONSTRAINT "N°";
       public         postgres    false    198               �   x�5˱�0���+��vlJM$���1,ޠ�2�Ŀ���cݙ*\3����j������J.����7Pe}l��m�ix.ú�ݡ�ϰk�և�����4��n�+�זq�X�w̿�T��B|~q!�         �   x�M�;�0Dk�[B�����H�$ �hX��Ȏ���9r1�Q��f��c�����kk��5B�m����&��)|�rо ��"���.>Ei�8�\��N��|̖��I_��2�=JQm�Z�dڇJy�(�ը:�*����Ks�
��I����4:(����:���E�Ep         �   x�uϽn� ��x�ۥ=��dL��*5R�.4E��S�#/Vl�8�:���I�@p�6e�]��V*���$���������~���,"ׯ�Ќ(�Ϡ@�ͧu}z��MŮ�]�m�5�݌�wgg�O�&-�@O�8�?R������Ci���.a�x��[�~i��GL6^�٦2ϧ���\Եxգ϶��O��	�OK��r�/��������NO�[�+~�0�� �?�A         �  x������0�y
�C�=vgoQ�*�v�BZ�R.�`��B�L�����ȋ�	����)�5���"��P���@k�A=F�%wmޖM���^�껝��Q���x'�ﲐ;�����G���'��y�}�X�ӟ�k���:"&���aN<Ѣ��}�d]H��9'�0&��YI�$�4������d>�1%���cdd؃��32%�R�9��N���2���g���M\�j��o]���P�w}���z���w��J���0#Qzτ�T/��";��J�[� �+�JFN�)V�Q��E���L��I�"���Ji�^h(.�Ͷ��_�ʱ����J4]��\���,��:_�>6�eQ�Jc/��ۦT���e����w;N)�$���˵<�E��~
U�l�-���٬�]uVs��7��O�b���
�y�qO��sR@�9#C��up- ' :ЙwZ��#���ّe��p�pcM�O8���vm����.e���K"{7�L�`ږo         !   x�3�t�H-,M�2��,J�M�+����� _�     