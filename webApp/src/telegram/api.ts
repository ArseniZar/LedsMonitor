import type { ChannelPost, ChatMember, ChatInfo, BotInfo, TelegramErrorResponse, TelegramSuccessResponse, TelegramUpdate } from './types'
import { HttpError, InvalidJsonError, NetworkError } from './errors'



export async function getBotInfo(token: string): Promise<BotInfo> {
  const response: Response = await fetch(`https://api.telegram.org/bot${token}/getMe`).catch(() => {
    throw new NetworkError('Network error occurred while fetching updates');
  });

  if (!response.ok) {
    const body: TelegramErrorResponse = await response.json().catch(() => { throw new InvalidJsonError(); });
    throw new HttpError(response.status, body.description);
  }

  const body: TelegramSuccessResponse<BotInfo> = await response.json().catch(() => { throw new InvalidJsonError(); });

  return body.result;
}



export async function getChatInfo(token: string, chatId: string): Promise<ChatInfo> {
  const response: Response = await fetch(`https://api.telegram.org/bot${token}/getChat?chat_id=${chatId}`).catch(() => {
    throw new NetworkError('Network error occurred while fetching updates');
  });

  if (!response.ok) {
    const body: TelegramErrorResponse = await response.json().catch(() => { throw new InvalidJsonError(); });
    throw new HttpError(response.status, body.description);
  }

  const body: TelegramSuccessResponse<ChatInfo> = await response.json().catch(() => { throw new InvalidJsonError(); });
  return body.result;
}



export async function getChatMemberInfo(token: string, chatId: string, userId: string): Promise<ChatMember> {
  const response: Response = await fetch(`https://api.telegram.org/bot${token}/getChatMember?chat_id=${chatId}&user_id=${userId}`).catch(() => {
    throw new NetworkError('Network error occurred while fetching updates');
  });

  if (!response.ok) {
    const body: TelegramErrorResponse = await response.json().catch(() => { throw new InvalidJsonError(); });
    throw new HttpError(response.status, body.description);
  }

  const body: TelegramSuccessResponse<ChatMember> = await response.json().catch(() => { throw new InvalidJsonError(); });
  return body.result;
}



export async function postCommandToTelegramChat(token: string, chatId: string, text: string): Promise<ChannelPost> {
  const response = await fetch(`https://api.telegram.org/bot${token}/sendMessage`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify({
      chat_id: chatId,
      text: text
    })
  })
    .catch(() => {
      throw new NetworkError('Network error occurred while fetching updates');
    });

  if (!response.ok) {
    const body: TelegramErrorResponse = await response.json().catch(() => { throw new InvalidJsonError(); });
    throw new HttpError(response.status, body.description);
  }

  const body: TelegramSuccessResponse<ChannelPost> = await response.json().catch(() => { throw new InvalidJsonError(); });
  return body.result;
}



export async function getTelegramUpdates(token: string, offset?: number): Promise<TelegramUpdate[]> {
  const url = new URL(`https://api.telegram.org/bot${token}/getUpdates`);
  if (offset !== undefined) {
    url.searchParams.set('offset', String(offset));
  }

  const response: Response = await fetch(url.toString()).catch(() => {
    throw new NetworkError('Network error occurred while fetching updates');
  });


  if (!response.ok) {
    const body: TelegramErrorResponse = await response.json().catch(() => { throw new InvalidJsonError(); });
    throw new HttpError(response.status, body.description);
  }

  const body: TelegramSuccessResponse<TelegramUpdate[]> = await response.json().catch(() => { throw new InvalidJsonError(); });
  return body.result;
}






