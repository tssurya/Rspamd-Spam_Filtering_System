-- Actually these regular expressions were obtained from SpamAssassin project, so they are licensed by apache license:
--
-- Licensed to the Apache Software Foundation (ASF) under one or more
-- contributor license agreements.  See the NOTICE file distributed with
-- this work for additional information regarding copyright ownership.
-- The ASF licenses this file to you under the Apache License, Version 2.0
-- (the "License"); you may not use this file except in compliance with
-- the License.  You may obtain a copy of the License at:
-- 
--     http://www.apache.org/licenses/LICENSE-2.0
-- 
-- Unless required by applicable law or agreed to in writing, software
-- distributed under the License is distributed on an "AS IS" BASIS,
-- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
-- See the License for the specific language governing permissions and
-- limitations under the License.
--
-- Fraud messages (Nigeria spam, viagra, etc)
local reconf = config['regexp']

local fraud_dbi = '/(?:\\bdollars?\\b|\\busd(?:ollars)?(?:[0-9]|\\b)|\\bus\\$|\\$[0-9,.]{6,}|\\$[0-9].{0,8}[mb]illion|\\$[0-9.,]{2,10} ?m|\\beuros?\\b|u[.]?s[.]? [0-9.]+ m)/irP'
local fraud_kjv = '/(?:claim|concerning) (?:the|this) money/irP'
local fraud_irj = '/(?:finance|holding|securit(?:ies|y)) (?:company|firm|storage house)/irP'
local fraud_neb = '/(?:government|bank) of nigeria/irP'
local fraud_xjr = '/(?:who was a|as a|an? honest|you being a|to any) foreigner/irP'
local fraud_dpr = '/\\b(?:(?:respond|reply) (?:urgently|immediately)|(?:urgent|immediate|earliest) (?:reply|response))\\b/irP'
local fraud_pts = '/\\b(?:ass?ass?inat(?:ed|ion)|murder(?:e?d)?|kill(?:ed|ing)\\b[^.]{0,99}\\b(?:war veterans|rebels?))\\b/irP'
local fraud_bep = '/\\b(?:bank of nigeria|central bank of|trust bank|apex bank|amalgamated bank)\\b/irP'
local fraud_tdp = '/\\b(?:business partner(?:s|ship)?|silent partner(?:s|ship)?)\\b/irP'
local fraud_gan = '/\\b(?:charles taylor|serena|abacha|gu[eйи]i|sese[- ]?seko|kabila)\\b/irP'
local fraud_irt = '/\\b(?:compliments? of the|dear friend|dear sir|yours faithfully|season\'?s greetings)\\b/irP'
local fraud_aon = '/\\b(?:confidential|private|alternate|alternative) (?:(?:e-? *)?mail)\\b/irP'
local fraud_wny = '/\\b(?:disburse?(?:ment)?|incurr?(?:ed)?|remunerr?at(?:ed?|ion)|remm?itt?(?:ed|ance|ing)?)\\b/irP'
local fraud_ipk = '/\\b(?:in|to|visit) your country\\b/irP'
local fraud_qxx = '/\\b(?:my name is|i am) (?:mrs?|engr|barrister|dr|prince(?:ss)?)[. ]/irP'
local fraud_iou = '/\\b(?:no risks?|risk-? *free|free of risks?|100% safe)\\b/irP'
local fraud_ezy = '/\\b(?:of|the) late president\\b/irP'
local fraud_mly = '/\\b(?:reply|respond)\\b[^.]{0,50}\\b(?:to|through)\\b[^.]{0,50}\\@\\b/irP'
local fraud_zfj = '/\\b(?:wife|son|brother|daughter) of the late\\b/irP'
local fraud_kdt = '/\\bU\\.?S\\.?(?:D\\.?)?\\s*(?:\\$\\s*)?(?:\\d+,\\d+,\\d+|\\d+\\.\\d+\\.\\d+|\\d+(?:\\.\\d+)?\\s*milli?on)/irP'
local fraud_ulk = '/\\baffidavits?\\b/irP'
local fraud_bgp = '/\\battached to ticket number\\b/irP'
local fraud_fbi = '/\\bdisburs/irP'
local fraud_jbu = '/\\bforeign account\\b/irP'
local fraud_yww = '/\\bfurnish you with\\b/irP'
local fraud_jyg = '/\\bgive\\s+you .{0,15}(?:fund|money|total|sum|contact|percent)\\b/irP'
local fraud_xvw = '/\\bhonest cooperation\\b/irP'
local fraud_uuy = '/\\blegitimate business(?:es)?\\b/irP'
local fraud_snt = '/\\blocate(?: .{1,20})? extended relative/irP'
local fraud_ltx = '/\\bmilli?on (?:.{1,25} thousand\\s*)?(?:(?:united states|u\\.?s\\.?) dollars|(?i:U\\.?S\\.?D?))\\b/irP'
local fraud_jnb = '/\\boperat(?:e|ing)\\b[^.]{0,99}\\b(?:for(?:ei|ie)gn|off-? ?shore|over-? ?seas?) (?:bank )?accounts?\\b/irP'
local fraud_qfy = '/\\bover-? *(?:invoiced?|cost(?:s|ing)?)\\b/irP'
local fraud_wdr = '/\\bprivate lawyer\\b/irP'
local fraud_wfc = '/\\bsecur(?:e|ing) (?:the )?(?:funds?|monies)\\b/irP'
local fraud_aum = '/\\bthe desk of\\b/irP'
local fraud_mcq = '/\\btransaction\\b.{1,30}\\b(?:magnitude|diplomatic|strict|absolute|secret|confiden(?:tial|ce)|guarantee)/irP'
local fraud_etx = '/\\byour\\b[^.]{0,99}\\b(?:contact (?:details|information)|private (?:e?[- ]?mail|telephone|tel|phone|fax))\\b/irP'
local fraud_pvn = '/as the beneficiary/irP'
local fraud_fvu = '/award notification/irP'
local fraud_ckf = '/computer ballot system/irP'
local fraud_fcw = '/fiduciary agent/irP'
local fraud_mqo = '/foreign (?:business partner|customer)/irP'
local fraud_tcc = '/foreign (?:offshore )?(?:bank|account)/irP'
local fraud_gbw = '/god gives .{1,10}second chance/irP'
local fraud_nrg = '/i am contacting you/irP'
local fraud_rlx = '/lott(?:o|ery) (?:co,?ordinator|international)/irP'
local fraud_axf = '/magnanimity/irP'
local fraud_thj = '/modalit(?:y|ies)/irP'
local fraud_yqv = '/nigerian? (?:national|government)/irP'
local fraud_yja = '/over-invoice/irP'
local fraud_ypo = '/the total sum/irP'
local fraud_uoq = '/vital documents/irP'
reconf['ADVANCE_FEE_2'] = string.format('((%s) | (%s) | (%s)) & regexp_match_number(2, (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s))', reconf['R_UNDISC_RCPT'], reconf['R_BAD_CTE_7BIT'], reconf['R_NO_SPACE_IN_FROM'], fraud_kjv, fraud_irj, fraud_neb, fraud_xjr, fraud_ezy, fraud_zfj, fraud_kdt, fraud_bgp, fraud_fbi, fraud_jbu, fraud_jyg, fraud_xvw, fraud_snt, fraud_ltx, fraud_mcq, fraud_pvn, fraud_fvu, fraud_ckf, fraud_fcw, fraud_mqo, fraud_tcc, fraud_gbw, fraud_nrg, fraud_rlx, fraud_axf, fraud_thj, fraud_yqv, fraud_yja, fraud_ypo, fraud_uoq, fraud_dbi, fraud_bep, fraud_dpr, fraud_qxx, fraud_qfy, fraud_pts, fraud_tdp, fraud_gan, fraud_ipk, fraud_aon, fraud_wny, fraud_aum, fraud_wfc, fraud_yww, fraud_ulk, fraud_iou, fraud_jnb, fraud_irt, fraud_etx, fraud_wdr, fraud_uuy, fraud_mly)
reconf['ADVANCE_FEE_3'] = string.format('((%s) | (%s) | (%s)) & regexp_match_number(3, (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s), (%s))', reconf['R_UNDISC_RCPT'], reconf['R_BAD_CTE_7BIT'], reconf['R_NO_SPACE_IN_FROM'], fraud_kjv, fraud_irj, fraud_neb, fraud_xjr, fraud_ezy, fraud_zfj, fraud_kdt, fraud_bgp, fraud_fbi, fraud_jbu, fraud_jyg, fraud_xvw, fraud_snt, fraud_ltx, fraud_mcq, fraud_pvn, fraud_fvu, fraud_ckf, fraud_fcw, fraud_mqo, fraud_tcc, fraud_gbw, fraud_nrg, fraud_rlx, fraud_axf, fraud_thj, fraud_yqv, fraud_yja, fraud_ypo, fraud_uoq, fraud_dbi, fraud_bep, fraud_dpr, fraud_qxx, fraud_qfy, fraud_pts, fraud_tdp, fraud_gan, fraud_ipk, fraud_aon, fraud_wny, fraud_aum, fraud_wfc, fraud_yww, fraud_ulk, fraud_iou, fraud_jnb, fraud_irt, fraud_etx, fraud_wdr, fraud_uuy, fraud_mly)
