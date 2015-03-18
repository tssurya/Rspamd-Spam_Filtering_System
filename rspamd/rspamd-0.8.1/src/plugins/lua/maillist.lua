-- Module for checking mail list headers

local symbol = 'MAILLIST'
local rspamd_logger = require "rspamd_logger"
-- EZMLM
-- Mailing-List: .*run by ezmlm
-- Precedence: bulk
-- List-Post: <mailto:
-- List-Help: <mailto:
-- List-Unsubscribe: <mailto:[a-zA-Z\.-]+-unsubscribe@
-- List-Subscribe: <mailto:[a-zA-Z\.-]+-subscribe@
local function check_ml_ezmlm(task)
	-- Mailing-List
	local header = task:get_header('mailing-list')
	if not header or not string.find(header, 'ezmlm$') then
		return false
	end
	-- Precedence
	header = task:get_header('precedence')
	if not header or not string.match(header, '^bulk$') then
		return false
	end
	-- Other headers
	header = task:get_header('list-post')
	if not header or not string.find(header, '^<mailto:') then
		return false
	end
	header = task:get_header('list-help')
	if not header or not string.find(header, '^<mailto:') then
		return false
	end
	-- Subscribe and unsubscribe
	header = task:get_header('list-subscribe')
	if not header or not string.find(header, '<mailto:[a-zA-Z.-]+-subscribe@') then
		return false
	end
	header = task:get_header('list-unsubscribe')
	if not header or not string.find(header, '<mailto:[a-zA-Z.-]+-unsubscribe@') then
		return false
	end

	return true
end

-- MailMan (the gnu mailing list manager)
-- Precedence: bulk [or list for v2]
-- List-Help: <mailto:
-- List-Post: <mailto:
-- List-Subscribe: .*<mailto:.*=subscribe>
-- List-Id: 
-- List-Unsubscribe: .*<mailto:.*=unsubscribe>
-- List-Archive: 
-- X-Mailman-Version: \d
local function check_ml_mailman(task)
	-- Mailing-List
	local header = task:get_header('x-mailman-version')
	if not header or not string.find(header, '^%d') then
		return false
	end
	-- Precedence
	header = task:get_header('precedence')
	if not header or (not string.match(header, '^bulk$') and not string.match(header, '^list$')) then
		return false
	end
	-- For reminders we have other headers than for normal messages
	header = task:get_header('x-list-administrivia')
	local subject = task:get_header('subject')
	if (header and string.find(header, 'yes')) or (subject and string.find(subject, 'mailing list memberships reminder$')) then
		if not task:get_header('errors-to') or not task:get_header('x-beenthere') then
			return false
		end
		header = task:get_header('x-no-archive')
		if not header or not string.find(header, 'yes') then
			return false
		end
		return true
	end

	-- Other headers
	header = task:get_header('list-id')
	if not header then
		return false
	end
	header = task:get_header('list-post')
	if not header or not string.find(header, '^<mailto:') then
		return false
	end
	header = task:get_header('list-help')
	if not header or not string.find(header, '^<mailto:') then
		return false
	end
	-- Subscribe and unsubscribe
	header = task:get_header('list-subscribe')
	if not header or not string.find(header, '<mailto:.*=subscribe>') then
		return false
	end
	header = task:get_header('list-unsubscribe')
	if not header or not string.find(header, '<mailto:.*=unsubscribe>') then
		return false
	end

	return true

end

-- Subscribe.ru
-- Precedence: normal
-- List-Id: <.*.subscribe.ru>
-- List-Help: <http://subscribe.ru/catalog/.*>
-- List-Subscribe: <mailto:.*-sub@subscribe.ru>
-- List-Unsubscribe: <mailto:.*-unsub@subscribe.ru>
-- List-Archive:  <http://subscribe.ru/archive/.*>
-- List-Owner: <mailto:.*-owner@subscribe.ru>
-- List-Post: NO
local function check_ml_subscriberu(task)
	-- List-Id
	local header = task:get_header('list-id')
	if not header or not string.find(header, '^<.*%.subscribe%.ru>$') then
		return false
	end
	-- Precedence
	header = task:get_header('precedence')
	if not header or not string.match(header, '^normal$') then
		return false
	end
	-- Other headers
	header = task:get_header('list-archive')
	if not header or not string.find(header, '^<http://subscribe.ru/archive/.*>$') then
		return false
	end
	header = task:get_header('list-owner')
	if not header or not string.find(header, '^<mailto:.*-owner@subscribe.ru>$') then
		return false
	end
	header = task:get_header('list-help')
	if not header or not string.find(header, '^<http://subscribe.ru/catalog/.*>$') then
		return false
	end
	-- Subscribe and unsubscribe
	header = task:get_header('list-subscribe')
	if not header or not string.find(header, '^<mailto:.*-sub@subscribe.ru>$') then
		return false
	end
	header = task:get_header('list-unsubscribe')
	if not header or not string.find(header, '^<mailto:.*-unsub@subscribe.ru>$') then
		return false
	end

	return true

end

-- RFC 2369 headers
local function check_rfc2369(task)
	local header = task:get_header('List-Id')
	if not header then
		return false
	end
	header = task:get_header('List-Unsubscribe')
	if not header or not string.find(header, '^^<.+>$') then
		return false
	end
	header = task:get_header('List-Subscribe')
	if not header or not string.find(header, '^^<.+>$') then
		return false
	end
	
	return true
end

-- RFC 2919 headers
local function check_rfc2919(task)
	local header = task:get_header('List-Id')
	if not header or not string.find(header, '^<.+>$') then
		return false
	end
	
	return check_rfc2369(task)
end

-- Google groups detector
-- header exists X-Google-Loop
-- RFC 2919 headers exist
--
local function check_ml_googlegroup(task)
	local header = task:get_header('X-Google-Loop')
	
	if not header then
		return false
	end

	return check_rfc2919(task)
end

-- Majordomo detector
-- Check Sender for owner- or -owner
-- Check Precendence for 'Bulk' or 'List'
--
-- And nothing more can be extracted :(
local function check_ml_majordomo(task)
	local header = task:get_header('Sender')
	if not header or (not string.find(header, '^owner-.*$') and not string.find(header, '^.*-owner$')) then
		return false
	end
	
	local header = task:get_header('Precedence')
	if not header or (header ~= 'list' and header ~= 'bulk') then
		return false
	end

	return true
end

-- CGP detector
-- X-Listserver = CommuniGate Pro LIST
-- RFC 2919 headers exist
--
local function check_ml_cgp(task)
	local header = task:get_header('X-Listserver')
	
	if not header or header ~= 'CommuniGate Pro LIST' then
		return false
	end

	return check_rfc2919(task)
end

local function check_maillist(task)
	if check_ml_ezmlm(task) then
		task:insert_result(symbol, 1, 'ezmlm')
	elseif check_ml_mailman(task) then
		task:insert_result(symbol, 1, 'mailman')
	elseif check_ml_subscriberu(task) then
		task:insert_result(symbol, 1, 'subscribe.ru')
	elseif check_ml_googlegroup(task) then
		task:insert_result(symbol, 1, 'googlegroups')
	elseif check_ml_majordomo(task) then
		task:insert_result(symbol, 1, 'majordomo')
	elseif check_ml_cgp(task) then
		task:insert_result(symbol, 1, 'cgp')
	end
end
-- Registration
if type(rspamd_config.get_api_version) ~= 'nil' then
	if rspamd_config:get_api_version() >= 1 then
		rspamd_config:register_module_option('maillist', 'symbol', 'string')
	end
end
-- Configuration
local opts =  rspamd_config:get_all_opt('maillist')if opts then
	if opts['symbol'] then
		symbol = opts['symbol'] 
		rspamd_config:register_symbol(symbol, 1.0, check_maillist)
	end
end
