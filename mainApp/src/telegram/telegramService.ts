import emitter from '../basic/eventBus';
import { getBotInfo, getChatInfo, getChatMemberInfo, getTelegramUpdates, postCommandToTelegramChat } from './api';
import { NetworkError, HttpError, InvalidJsonError } from './errors';
import type { User, Chat, ChatMember, Update } from '@telegraf/types';

export interface ResultData<T> {
    status: boolean;
    value?: T | T[];
    alertMessage?: string;
}

export async function validateChatId(token: string, chatId: string, userId: string): Promise<ResultData<unknown>> {
    try {
        const chatInfo: Chat = await getChatInfo(token, chatId);

        if (chatInfo.type !== 'channel') {
            return handleTelegramError('Chat is not a channel');
        }

        const member: ChatMember = await getChatMemberInfo(token, chatId, userId);

        if (member.status === 'administrator' || member.status === 'creator') {
            return { status: true };
        } else {
            return handleTelegramError('You are not an administrator in this chat');
        }

    } catch (error: any) {
        return handleTelegramError(error);
    }
}


export async function getChat(token: string, chatId: string): Promise<ResultData<Chat>> {
    try {
        const chatInfo: Chat = await getChatInfo(token, chatId);
        return { status: true, value: chatInfo };

    } catch (error: any) {
        return handleTelegramError(error);

    }
}


export async function validateToken(token: string): Promise<ResultData<unknown>> {
    try {
        console.log(token);
        const user: User = await getBotInfo(token);
        
        if (user.is_bot) {
            return { status: true };
        } else {
            return handleTelegramError('Provided token does not belong to a bot');
        }
    }
    catch (error: any) {
        return handleTelegramError(error);
    }
}

export async function getBot(token: string): Promise<ResultData<User>> {
    try {
        const user: User = await getBotInfo(token);
        if (user.is_bot) {
            return { status: true, value: user };
        } else {
            return handleTelegramError('The provided token does not belong to a bot user.');
        }
    }
    catch (error: any) {
        return handleTelegramError(error);
    }
}


export async function getOffset(token: string, offset?: number, allowed_updates?: string[]): Promise<ResultData<number>> {
    try {
        const result: Update[] = await getTelegramUpdates(token, offset, allowed_updates);
        if (result.length === 0) {
            return { status: true, value: 0 };
        }
        return { status: true, value: result[result.length - 1].update_id };

    } catch (error: any) {
        return handleTelegramError(error);
    }
}

export async function sendMessage(token: string, chatId: string, text: string): Promise<ResultData<unknown>> {
    try {
        await postCommandToTelegramChat(token, chatId, text);
        return { status: true };
    } catch (error: any) {
        return handleTelegramError(error);
    }
}


export async function getUpdates(token: string, offset?: number, allowed_updates?: string[]): Promise<ResultData<Update>> {
    try {
        const result: Update[] = await getTelegramUpdates(token, offset, allowed_updates);
        return { status: true, value: result };

    } catch (error: any) {
        return handleTelegramError(error);
    }
}

function handleTelegramError<T>(errorOrMessage: any): ResultData<T> {
    let alertMessage = 'Unknown error occurred';

    if (typeof errorOrMessage === 'string') {
        alertMessage = errorOrMessage;
    } else if (errorOrMessage instanceof NetworkError) {
        alertMessage = 'Network error: Please check your internet connection and try again.';
    } else if (errorOrMessage instanceof HttpError) {
        switch (errorOrMessage.status) {
            case 400:
                alertMessage = 'Bad Request: Chat does not exist or the application is not a member.';
                break;
            case 401:
                alertMessage = 'Unauthorized: Invalid bot token.';
                break;
            case 403:
                alertMessage = 'Forbidden: Bot has no permission to perform this action.';
                break;
            case 429:
                alertMessage = 'Too Many Requests: You are sending requests too quickly. Please slow down.';
                break;
            default:
                if (errorOrMessage.status >= 500 && errorOrMessage.status < 600) {
                    alertMessage = `Server error (${errorOrMessage.status}): Telegram service temporarily unavailable. Try again later.`;
                } else {
                    alertMessage = `HTTP error ${errorOrMessage.status}: ${errorOrMessage.message}`;
                }
        }
    } else if (errorOrMessage instanceof InvalidJsonError) {
        alertMessage = `Invalid JSON response received: ${errorOrMessage.message}`;
    } else if (errorOrMessage instanceof Error) {
        alertMessage = `Unknown error: ${errorOrMessage.message}`;
    } else {
        alertMessage = `Unknown error: ${String(errorOrMessage)}`;
    }

    emitter.emit('alert', alertMessage);
    return { status: false, alertMessage };
}
