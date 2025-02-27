import { IChat } from "@/types"
import { create } from "zustand"

interface ChatStore {
    data: IChat | null,
    setCurrentChat: (chat: IChat) => void
}

export const useChatStore = create<ChatStore>()((set) => ({
    data: null,
    setCurrentChat: (chat: IChat) => set({ data: chat })
}))
