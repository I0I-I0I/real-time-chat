import { Button, Typography } from "@/components/UI"

import styles from "./ChatsItem.module.css"

import cls from "@/utils/cls"
import { ReactSVG } from "react-svg"

import { IChat } from "@/types"

interface ChatsItemProps {
    index: number | null
    className?: string
    data: IChat
    onClick?: (chat: IChat) => void
    onClickRemove?: (chatId: number) => void
}

export const ChatsItem = ({
    index,
    className = "",
    data,
    onClick = () => {},
    onClickRemove
}: ChatsItemProps) => {
    return (
        <li className={cls(styles.item, className)} aria-selected aria-label="Chat">
            <Button
                className={styles.button}
                onClick={() => onClick(data)}>
            <>
                <ReactSVG
                    className={styles.icon}
                    src={ data.img ? data.img : "/account.svg" }
                />
                <div className={styles.body}>
                    { index && <Typography tag="span" variant="text_tiny" className={styles.index}>{String(index)}</Typography> }
                    <Typography tag="h2" variant="title-4">{data.name}</Typography>
                    { data.lastMessage && <Typography tag="span" variant="text_tiny">{data.lastMessage}</Typography> }
                </div>
            </>
            </Button>
            { onClickRemove &&
                <Button className={styles.remove_button} onClick={() => onClickRemove(data.id)}>
                    X
                </Button>
            }
        </li>
    )
}
