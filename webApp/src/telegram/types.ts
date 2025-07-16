interface TelegramSuccessResponse<T> {
    ok: boolean;
    result: T;
};

interface TelegramErrorResponse {
    ok: false;
    error_code: number;
    description: string;
};

/*=======================================*/

type BotInfo = {
    id: number;
    is_bot: boolean;
    first_name: string;
    username: string;
    can_join_groups: boolean;
    can_read_all_group_messages: boolean;
    supports_inline_queries: boolean;
    can_connect_to_business: boolean;
    has_main_web_app: boolean;
};

/*=======================================*/

type User = {
    id: number;
    is_bot: boolean;
    first_name: string;
    username: string;
};

type ChatMember = {
    user: User;
    status: string;
    can_be_edited: boolean;
    can_manage_chat: boolean;
    can_change_info: boolean;
    can_post_messages: boolean;
    can_edit_messages: boolean;
    can_delete_messages: boolean;
    can_invite_users: boolean;
    can_restrict_members: boolean;
    can_promote_members: boolean;
    can_manage_video_chats: boolean;
    can_post_stories: boolean;
    can_edit_stories: boolean;
    can_delete_stories: boolean;
    is_anonymous: boolean;
    can_manage_voice_chats: boolean;
};

/*=======================================*/

type AcceptedGiftTypes = {
    unlimited_gifts: boolean;
    limited_gifts: boolean;
    unique_gifts: boolean;
    premium_subscription: boolean;
};

type ChatInfo = {
    id: number;
    title: string;
    type: 'channel';
    invite_link: string;
    can_send_gift: boolean;
    has_visible_history: boolean;
    can_send_paid_media: boolean;
    accepted_gift_types: AcceptedGiftTypes;
    message_auto_delete_time: number;
    available_reactions: any[]; 
    max_reaction_count: number;
    accent_color_id: number;
};


/*=======================================*/


type TelegramUpdate = {
    update_id: string;
    channel_post: ChannelPost;
};


type ChannelPost = {
    message_id: string;
    sender_chat: Chat;
    chat: Chat;
    date: string;
    text: string;
    entities: MessageEntity[];
};

type Chat = {
    id: string;
    title: string;
    type?: string;
};

type MessageEntity = {
    offset: string;
    length: string;
    type: string;
};


export type { TelegramSuccessResponse, TelegramErrorResponse, TelegramUpdate, ChatMember, ChannelPost, ChatInfo, Chat, BotInfo };