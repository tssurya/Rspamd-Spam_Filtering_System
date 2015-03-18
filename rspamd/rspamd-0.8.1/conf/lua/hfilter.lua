--
-- Copyright (c) 2013-2014, Alexey Savelyev
-- E-mail: info@homeweb.ru
-- WWW: http://homeweb.ru
--

-- Weight for checks_hellohost and checks_hello: 5 - very hard, 4 - hard, 3 - meduim, 2 - low, 1 - very low.
-- From HFILTER_HELO_* and HFILTER_HOSTNAME_* symbols the maximum weight is selected in case of their actuating.


--local dumper = require 'pl.pretty'.dump
local rspamd_regexp = require "rspamd_regexp"

local checks_hellohost = {
  ['[.-]dynamic[.-]'] = 5, ['dynamic[.-][0-9]'] = 5, ['[0-9][.-]?dynamic'] = 5, 
  ['[.-]dyn[.-]'] = 5, ['dyn[.-][0-9]'] = 5, ['[0-9][.-]?dyn'] = 5, 
  ['[.-]clients?[.-]'] = 5, ['clients?[.-][0-9]'] = 5, ['[0-9][.-]?clients?'] = 5, 
  ['[.-]dynip[.-]'] = 5, ['dynip[.-][0-9]'] = 5, ['[0-9][.-]?dynip'] = 5, 
  ['[.-]broadband[.-]'] = 5, ['broadband[.-][0-9]'] = 5, ['[0-9][.-]?broadband'] = 5, 
  ['[.-]broad[.-]'] = 5, ['broad[.-][0-9]'] = 5, ['[0-9][.-]?broad'] = 5, 
  ['[.-]bredband[.-]'] = 5, ['bredband[.-][0-9]'] = 5, ['[0-9][.-]?bredband'] = 5, 
  ['[.-]nat[.-]'] = 5, ['nat[.-][0-9]'] = 5, ['[0-9][.-]?nat'] = 5, 
  ['[.-]pptp[.-]'] = 5, ['pptp[.-][0-9]'] = 5, ['[0-9][.-]?pptp'] = 5, 
  ['[.-]pppoe[.-]'] = 5, ['pppoe[.-][0-9]'] = 5, ['[0-9][.-]?pppoe'] = 5, 
  ['[.-]ppp[.-]'] = 5, ['ppp[.-][0-9]'] = 5, ['[0-9][.-]?ppp'] = 5, 
  ['[.-][a|x]?dsl[.-]'] = 4, ['[a|x]?dsl[.-]?[0-9]'] = 4, ['[0-9][.-]?[a|x]?dsl'] = 4, 
  ['[.-][a|x]?dsl-dynamic[.-]'] = 5, ['[a|x]?dsl-dynamic[.-]?[0-9]'] = 5, ['[0-9][.-]?[a|x]?dsl-dynamic'] = 5, 
  ['[.-][a|x]?dsl-line[.-]'] = 4, ['[a|x]?dsl-line[.-]?[0-9]'] = 4, ['[0-9][.-]?[a|x]?dsl-line'] = 4, 
  ['[.-]dhcp[.-]'] = 5, ['dhcp[.-][0-9]'] = 5, ['[0-9][.-]?dhcp'] = 5, 
  ['[.-]catv[.-]'] = 5, ['catv[.-][0-9]'] = 5, ['[0-9][.-]?catv'] = 5, 
  ['[.-]wifi[.-]'] = 5, ['wifi[.-][0-9]'] = 5, ['[0-9][.-]?wifi'] = 5, 
  ['[.-]unused-addr[.-]'] = 3, ['unused-addr[.-][0-9]'] = 3, ['[0-9][.-]?unused-addr'] = 3, 
  ['[.-]dial-?up[.-]'] = 5, ['dial-?up[.-][0-9]'] = 5, ['[0-9][.-]?dial-?up'] = 5, 
  ['[.-]gprs[.-]'] = 5, ['gprs[.-][0-9]'] = 5, ['[0-9][.-]?gprs'] = 5, 
  ['[.-]cdma[.-]'] = 5, ['cdma[.-][0-9]'] = 5, ['[0-9][.-]?cdma'] = 5, 
  ['[.-]homeuser[.-]'] = 5, ['homeuser[.-][0-9]'] = 5, ['[0-9][.-]?homeuser'] = 5, 
  ['[.-]in-?addr[.-]'] = 4, ['in-?addr[.-][0-9]'] = 4, ['[0-9][.-]?in-?addr'] = 4, 
  ['[.-]pool[.-]'] = 4, ['pool[.-][0-9]'] = 4, ['[0-9][.-]?pool'] = 4, 
  ['[.-]cable[.-]'] = 3, ['cable[.-][0-9]'] = 3, ['[0-9][.-]?cable'] = 3,
  ['[.-]host[.-]'] = 2, ['host[.-][0-9]'] = 2, ['[0-9][.-]?host'] = 2,
  ['[.-]customers[.-]'] = 1, ['customers[.-][0-9]'] = 1, ['[0-9][.-]?customers'] = 1
}

local checks_hello = {
  ['localhost$'] = 5,
  ['^(dsl)?(device|speedtouch)\\.lan$'] = 5,
  ['\\.(lan|local|home|localdomain|intra|in-addr.arpa|priv|online|user|veloxzon)$'] = 5,
  ['^\\[*0\\.'] = 5, ['^\\[*::1\\]*'] = 5, --loopback ipv4, ipv6
  ['^\\[*127\\.'] = 5, ['^\\[*10\\.'] = 5, ['^\\[*172\\.16\\.'] = 5, ['^\\[*192\\.168\\.'] = 5, --local ipv4
  ['^\\[*fe[89ab][0-9a-f]::'] = 5, ['^\\[*fe[cdf][0-9a-f]:'] = 5, --local ipv6 (fe80:: - febf::, fec0:: - feff::)
  ['^\\[*2001:db8::'] = 5, --reserved RFC 3849 for ipv6
  ['^\\[*fc00::'] = 5, ['^\\[*ffxx::'] = 5, --unicast, multicast ipv6
  ['^\\[*\\d+[x.-]\\d+[x.-]\\d+[x.-]\\d+\\]*$'] = 4, ['^\\[*\\d+:'] = 4 --bareip ipv4, ipv6
}

local config = {
  ['helo_enabled'] = false,
  ['hostname_enabled'] = false,
  ['from_enabled'] = false,
  ['mid_enabled'] = false,
  ['url_enabled'] = false
}

local function trim1(s)
  return (s:gsub("^%s*(.-)%s*$", "%1"))
end

local function check_regexp(str, regexp_text)
  local re = rspamd_regexp.create_cached(regexp_text, 'i')
  if re:match(str) then return true end
  return false
end

local function split(str, delim, maxNb)
  -- Eliminate bad cases...
  if string.find(str, delim) == nil then
    return { str }
  end
  if maxNb == nil or maxNb < 1 then
    maxNb = 0    -- No limit
  end
  local result = {}
  local pat = "(.-)" .. delim .. "()"
  local nb = 0
  local lastPos
  for part, pos in string.gmatch(str, pat) do
    nb = nb + 1
    result[nb] = part
    lastPos = pos
    if nb == maxNb then break end
  end
  -- Handle the last field
  if nb ~= maxNb then
    result[nb + 1] = string.sub(str, lastPos)
  end
  return result
end

local function check_fqdn(domain)
  if check_regexp(domain, '(?=^.{4,255}$)(^((?!-)[a-zA-Z0-9-]{1,63}(?<!-)\\.)+[a-zA-Z]{2,63}$)') then
    return true
  end
  return false
end

-- host: host for check
-- symbol_suffix: suffix for symbol
-- eq_ip: ip for comparing or empty string
-- eq_host: host for comparing or empty string
local function check_host(task, host, symbol_suffix, eq_ip, eq_host)

  local function check_host_cb_mx_a(resolver, to_resolve, results, err)
    task:inc_dns_req()
    if not results then
      task:insert_result('HFILTER_' .. symbol_suffix .. '_NORESOLVE_MX', 1.0)
    end
  end
  local function check_host_cb_mx(resolver, to_resolve, results, err)
    task:inc_dns_req()
    if not results then
      task:insert_result('HFILTER_' .. symbol_suffix .. '_NORES_A_OR_MX', 1.0)
    else
      for _,mx in pairs(results) do
        if mx['name'] then
          task:get_resolver():resolve_a(task:get_session(), task:get_mempool(), mx['name'], check_host_cb_mx_a)
        end
      end
    end
  end
  local function check_host_cb_a(resolver, to_resolve, results, err)
    task:inc_dns_req()

    if not results then
      task:get_resolver():resolve_mx(task:get_session(), task:get_mempool(), host, check_host_cb_mx)
    elseif eq_ip ~= '' then
      for _,result in pairs(results) do 
        if result:to_string() == eq_ip then
          return true
        end
      end
      task:insert_result('HFILTER_' .. symbol_suffix .. '_IP_A', 1.0)
    end
  end

  if host then
    host = string.lower(host)
  else
    return false
  end
  if eq_host then
    eq_host = string.lower(eq_host)
  else
    eq_host = ''
  end

  if check_fqdn(host) then
    if eq_host == '' or eq_host ~= 'unknown' or eq_host ~= host then
      task:get_resolver():resolve_a(task:get_session(), task:get_mempool(), host, check_host_cb_a)
    end
  else
    task:insert_result('HFILTER_' .. symbol_suffix .. '_NOT_FQDN', 1.0)
  end

  return true
end

--
local function hfilter(task)

  --IP--
  local ip = false
  local rip = task:get_from_ip()
  if rip and rip:is_valid() then
    ip = rip:to_string()
  end

  --HOSTNAME--
  local hostname = task:get_hostname()

  --HELO--
  local helo = task:get_helo()

  --RULES--RULES--RULES--

  -- Check's HELO
  local weight_helo = 0
  if config['helo_enabled'] then
    if helo then
      -- Regexp check HELO (checks_hello)
      for regexp,weight in pairs(checks_hello) do
        if check_regexp(helo, regexp) then
          weight_helo = weight
          break
        end
      end

      -- Regexp check HELO (checks_hellohost)
      for regexp,weight in pairs(checks_hellohost) do
        if check_regexp(helo, regexp) then
          if weight > weight_helo then
            weight_helo = weight
          end
          break
        end
      end

      --FQDN check HELO
      if ip and helo then
        check_host(task, helo, 'HELO', ip, hostname)
      end
    end
  end

  -- Check's HOSTNAME
  if config['hostname_enabled'] then
    local weight_hostname = 0
    if hostname then
      -- Check regexp HOSTNAME
      if hostname == 'unknown' then
        task:insert_result('HFILTER_HOSTNAME_UNKNOWN', 1.00)
      else
        for regexp,weight in pairs(checks_hellohost) do
          if check_regexp(hostname, regexp) then
            weight_hostname = weight
            break
          end
        end
      end
    end

    --Insert weight's for HELO or HOSTNAME
    if weight_helo > 0 and weight_helo >= weight_hostname then
      task:insert_result('HFILTER_HELO_' .. weight_helo, 1.0)
    elseif weight_hostname > 0 and weight_hostname > weight_helo then
      task:insert_result('HFILTER_HOSTNAME_' .. weight_hostname, 1.0)
    end
  end
  
  if config['from_enabled'] then
    -- MAILFROM checks --
    local from = task:get_from(1)
    if from then
      --FROM host check
      for _,fr in ipairs(from) do
        local fr_split = split(fr['addr'], '@', 0)
        if table.maxn(fr_split) == 2 then
          check_host(task, fr_split[2], 'FROMHOST', '', '')
        end
      end
    end
  end

  --Message ID host check
  if config['mid_enabled'] then
    local message_id = task:get_message_id()
    if message_id then
      local mid_split = split(message_id, '@', 0)
      if table.maxn(mid_split) == 2 and not string.find(mid_split[2], 'local') then
        check_host(task, mid_split[2], 'MID', '', '')
      end
    end
  end
  
  -- Links checks
  if config['url_enabled'] then
    local parts = task:get_text_parts()
    if parts then
      --One text part--
      local total_parts_len = 0
      local text_parts_count = 0
      local selected_text_part = nil
      for _,p in ipairs(parts) do
        total_parts_len = total_parts_len + p:get_length()

        if not p:is_html() then
          text_parts_count = text_parts_count + 1
          selected_text_part = p
        end
      end
      if total_parts_len > 0 then
        local urls = task:get_urls()
        if urls then
          local total_url_len = 0
          for _,url in ipairs(urls) do
            total_url_len = total_url_len + url:get_length()
          end
          if total_url_len > 0 then
            if total_url_len + 7 > total_parts_len then
              task:insert_result('HFILTER_URL_ONLY', 1.00)
            elseif text_parts_count == 1 and selected_text_part and selected_text_part:get_length() < 1024 then
              -- We got a single text part with the total length < 1024 symbols.
              local part_text = selected_text_part:get_content()
              if part_text and not string.find(trim1(part_text), "\n") then
                task:insert_result('HFILTER_URL_ONELINE', 1.00)
              end
            end
          end
        end
      end
    end
  end

  return false
end

local symbols_enabled = {}

local symbols_helo = {
  "HFILTER_HELO_1",
  "HFILTER_HELO_2",
  "HFILTER_HELO_3", 
  "HFILTER_HELO_4",
  "HFILTER_HELO_5",
  "HFILTER_HELO_NORESOLVE_MX", 
  "HFILTER_HELO_NORES_A_OR_MX",
  "HFILTER_HELO_IP_A",
  "HFILTER_HELO_NOT_FQDN"
}
local symbols_hostname = {
  "HFILTER_HOSTNAME_1",
  "HFILTER_HOSTNAME_2",
  "HFILTER_HOSTNAME_3",
  "HFILTER_HOSTNAME_4",
  "HFILTER_HOSTNAME_5",
  "HFILTER_HOSTNAME_UNKNOWN"
}
local symbols_mid = {
  "HFILTER_MID_NORESOLVE_MX",
  "HFILTER_MID_NORES_A_OR_MX",
  "HFILTER_MID_NOT_FQDN",
  "HFILTER_MID_NOT_FQDN"
}
local symbols_url = {
  "HFILTER_URL_ONLY",
  "HFILTER_URL_ONELINE"
}
local symbols_from = {
  "HFILTER_FROMHOST_NORESOLVE_MX",
  "HFILTER_FROMHOST_NORES_A_OR_MX",
  "HFILTER_FROMHOST_NOT_FQDN"
}

local opts = rspamd_config:get_all_opt('hfilter')
if opts then
  for k,v in pairs(opts) do
    config[k] = v
  end
end

local function append_t(t, a)
  for _,v in ipairs(a) do table.insert(t, v) end
end
if config['helo_enabled'] then
  append_t(symbols_enabled, symbols_helo)
end
if config['hostname_enabled'] then
  append_t(symbols_enabled, symbols_hostname)
end
if config['from_enabled'] then
  append_t(symbols_enabled, symbols_from)
end
if config['mid_enabled'] then
  append_t(symbols_enabled, symbols_mid)
end
if config['url_enabled'] then
  append_t(symbols_enabled, symbols_url)
end

--dumper(symbols_enabled)
if table.maxn(symbols_enabled) > 0 then
  rspamd_config:register_symbols(hfilter, 1.0, "HFILTER", symbols_enabled);
end
